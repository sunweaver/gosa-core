<?php

class stats 
{
    
    static protected $last_cpu_load = "";
    static protected $last_cpu_load_stamp = 0;


    static function checkDatabase()
    {
        $TABLE_NAME = 'stats';
    
        // Check for modules
        // php5-sqlite


        // Try to create database, if it exists just open it.
        $res = sqlite_open('/var/spool/gosa/stats', 0666, $error);
        if(!$res){
            return($res);
        }

        // Delete Table 
        $query = "DROP TABLE '{$TABLE_NAME}'";
#        $ret = sqlite_query($query, $res);

        // List Tables an check if there is already everything we need.
        $query = "SELECT name FROM sqlite_master WHERE type='table' and name='{$TABLE_NAME}'";
        $ret = sqlite_query($query, $res);
        if(!count(sqlite_fetch_all($ret))){

            // Check for table existance    
            $query = "
                CREATE TABLE {$TABLE_NAME} (
                        ID              INTEGER PRIMARY KEY,
                        ACTID           INTEGER,
                        TYPE            TEXT,
                        PLUGIN          TEXT,
                        CATEGORY        TEXT,
                        ACTION          TEXT,
                        UUID            TEXT,
                        TIMESTAMP       INTEGER,
                        MTIMESTAMP      REAL,
                        DURATION        REAL,
                        AMOUNT          INTEGER,
                        MEMORY_USAGE    INTEGER,
                        CPU_LOAD        FLOAT,
                        INFO            BLOB
                        )";
            $ret = sqlite_query($query, $res);
        }

        return($res);
    }



    static function log($type, $plugin, $category, $action, $amount = 1, $duration = 0, $info ='')
    {
        global $config;
        global $clicks;
        $type           = sqlite_escape_string($type);
        $plugin         = sqlite_escape_string($plugin);
        $action         = sqlite_escape_string($action);
        $timestamp      = time();
        $mtimestamp     = microtime(TRUE);
        $uuid           = $config->getGOsaUUID();
        $amount         = sqlite_escape_string($amount);
        $duration       = sqlite_escape_string($duration);
        $info           = sqlite_escape_string($info);
        $clicks         = sqlite_escape_string($clicks);
        $memory_usage   = sqlite_escape_string(stats::get_memory_usage());
        $cpu_load       = sqlite_escape_string(stats::get_cpu_load());

        $tmp = array();
        foreach($category as $cat){
            $tmp[] = trim($cat, '\/,; ');
        }
        $category = sqlite_escape_string(implode($tmp, ', '));

        $res = stats::checkDatabase();
        $TABLE_NAME = 'stats';
        $query = "
            INSERT INTO {$TABLE_NAME}
                (ACTID, TYPE, PLUGIN, CATEGORY, ACTION, UUID, MTIMESTAMP, TIMESTAMP, AMOUNT, DURATION, MEMORY_USAGE, CPU_LOAD, INFO) 
            VALUES 
                ('{$clicks}','{$type}','{$plugin}','{$category}','{$action}','{$uuid}',
                    '{$mtimestamp}','{$timestamp}','{$amount}','{$duration}','{$memory_usage}','{$cpu_load}','{$info}')";
        sqlite_query($query, $res);
    }

    static function get_memory_usage()
    {
        return(memory_get_usage());
    }

    static function get_cpu_load()
    {
        $cur = time();
        if(empty(stats::$last_cpu_load) || (($cur - stats::$last_cpu_load_stamp) >=2 )){
            list($one, $five, $ten) =preg_split("/ /",shell_exec('cat /proc/loadavg'));
            stats::$last_cpu_load = $one;
            stats::$last_cpu_load_stamp = $cur;
        }
        return(stats::$last_cpu_load);
    }
    
    static function show()
    {
        $res = stats::checkDatabase();
        $TABLE_NAME = 'stats';
        $query = "SELECT * FROM {$TABLE_NAME} ORDER BY MTIMESTAMP";
        $query = "SELECT PLUGIN, ACTION, MAX(DURATION) as 'DURATION' FROM {$TABLE_NAME} WHERE ACTION='modify' GROUP BY PLUGIN,ACTION ";
        $query = "SELECT * FROM {$TABLE_NAME} ORDER BY ID DESC LIMIT 30";
        $query = "SELECT * FROM {$TABLE_NAME} WHERE plugin != 'LDAP' ORDER BY ID DESC LIMIT 30";
        $ret = sqlite_query($query, $res);
        echo "<pre>";

        $colSize = 16;

        $title = FALSE;
        foreach(sqlite_fetch_all($ret) as $entry){
            if(!$title){
                foreach($entry as $key => $str) {
                    if(is_numeric($key)) continue;
                    echo str_pad($key,$colSize,' ')."|";
                }
                echo "\n";
                foreach($entry as $key => $str) {
                    if(is_numeric($key)) continue;
                    echo str_pad('',$colSize,'-')."-";
                }
                echo "\n";
                $title = TRUE;
            }
 
            foreach($entry as $key => $str){
                if(is_numeric($key)) continue;
                if($key == "DURATION" || $key == "MTIMESTAMP" || $key == "CPU_LOAD"){
                    $str = sprintf("%0.4f", preg_replace("/,/",".",$str));
                    echo str_pad($str,$colSize,' ', STR_PAD_LEFT)."|"; 
                }else{
                    echo str_pad($str,$colSize,' ')."|"; 
                }
            }
            echo "\n";
        }
        echo sqlite_error_string($ret);

        echo "\n------ \n";
        echo "Time spent per plugin-category \n";
        echo "------ \n";

        $query = "
            SELECT SUM(DURATION) AS DUR, CATEGORY 
            FROM {$TABLE_NAME}
            WHERE plugin != 'LDAP' 
            GROUP BY CATEGORY 
            ORDER BY DUR DESC LIMIT 10";
        $ret = sqlite_query($query, $res);

        $colSize = 16;
        $title = FALSE;
        foreach(sqlite_fetch_all($ret) as $entry){
            foreach($entry as $key => $str){
                if(is_numeric($key)) continue;
                echo str_pad($str,$colSize,' ')."|"; 
            }
            echo "\n";
        }


        echo sqlite_error_string($ret);

        echo "\n------ \n";
        echo "Time spent per plugin \n";
        echo "------ \n";

        $query = "
            SELECT SUM(DURATION) AS DUR, PLUGIN, UUID 
            FROM {$TABLE_NAME}
            WHERE plugin != 'LDAP' 
            GROUP BY PLUGIN, UUID 
            ORDER BY DUR DESC LIMIT 10";
        $ret = sqlite_query($query, $res);

        $colSize = 16;
        $title = FALSE;
        foreach(sqlite_fetch_all($ret) as $entry){
            foreach($entry as $key => $str){
                if(is_numeric($key)) continue;
                echo str_pad($str,$colSize,' ')."|"; 
            }
            echo "\n";
        }
        echo sqlite_error_string($ret);

 # * Anzahl von Objekttypen
 # * Anzahl der Löschungen pro Objekttyp
 # * Anzahl der Erzeugungen pro Objekttyp
 # * Anzahl der Bearbeitungen pro Objekttyp
 # * Anzahl der Verschiebungen pro Objekttyp
 # * Anzahl der Mehrfachbearbeitungen pro Objekttyp
 # * Antwortzeiten pro aktion
 # * Anzahl der Passwortänderungen
 # * Anzahl der unterschiedlichen Anmeldungen


        echo "\n------ \n";
        echo "Actions done per plugin \n";
        echo "------ \n";

        $query = "
            SELECT COUNT(ACTION) as CNT, ACTION, PLUGIN 
            FROM {$TABLE_NAME}
            WHERE   TYPE = 'plugin'
             AND    PLUGIN != 'LDAP'
            GROUP BY ACTION,PLUGIN 
            ORDER BY CNT DESC LIMIT 30";
        $ret = sqlite_query($query, $res);

        $colSize = 16;
        $title = FALSE;
        foreach(sqlite_fetch_all($ret) as $entry){
            foreach($entry as $key => $str){
                if(is_numeric($key)) continue;
                echo str_pad($str,$colSize,' ')."|"; 
            }
            echo "\n";
        }
        echo sqlite_error_string($ret);

        echo "\n------ \n";
        echo "'create' actions done per plugin  (5 most)\n";
        echo "------ \n";

        $query = "
            SELECT COUNT(ACTION) as CNT, ACTION, PLUGIN 
            FROM {$TABLE_NAME}
            WHERE   TYPE = 'plugin'
             AND    PLUGIN != 'LDAP'
             AND    ACTION = 'create'
            GROUP BY ACTION,PLUGIN 
            ORDER BY CNT DESC LIMIT 5";
        $ret = sqlite_query($query, $res);

        $colSize = 16;
        $title = FALSE;
        foreach(sqlite_fetch_all($ret) as $entry){
            foreach($entry as $key => $str){
                if(is_numeric($key)) continue;
                echo str_pad($str,$colSize,' ')."|"; 
            }
            echo "\n";
        }
        echo sqlite_error_string($ret);

        echo "\n------ \n";
        echo "'move' actions done per plugin  (5 most)\n";
        echo "------ \n";

        $query = "
            SELECT COUNT(ACTION) as CNT, ACTION, PLUGIN 
            FROM {$TABLE_NAME}
            WHERE   TYPE = 'plugin'
             AND    PLUGIN != 'LDAP'
             AND    ACTION = 'move'
            GROUP BY ACTION,PLUGIN 
            ORDER BY CNT DESC LIMIT 5";
        $ret = sqlite_query($query, $res);

        $colSize = 16;
        $title = FALSE;
        foreach(sqlite_fetch_all($ret) as $entry){
            foreach($entry as $key => $str){
                if(is_numeric($key)) continue;
                echo str_pad($str,$colSize,' ')."|"; 
            }
            echo "\n";
        }
        echo sqlite_error_string($ret);

        echo "\n------ \n";
        echo "'view' actions done per plugin  (5 most)\n";
        echo "------ \n";

        $query = "
            SELECT COUNT(ACTION) as CNT, ACTION, PLUGIN 
            FROM {$TABLE_NAME}
            WHERE   TYPE = 'plugin'
             AND    PLUGIN != 'LDAP'
             AND    ACTION = 'view'
            GROUP BY ACTION,PLUGIN 
            ORDER BY CNT DESC LIMIT 5";
        $ret = sqlite_query($query, $res);

        $colSize = 16;
        $title = FALSE;
        foreach(sqlite_fetch_all($ret) as $entry){
            foreach($entry as $key => $str){
                if(is_numeric($key)) continue;
                echo str_pad($str,$colSize,' ')."|"; 
            }
            echo "\n";
        }
        echo sqlite_error_string($ret);

        echo "\n------ \n";
        echo "'open' actions done per plugin  (5 most)\n";
        echo "------ \n";

        $query = "
            SELECT COUNT(ACTION) as CNT, ACTION, PLUGIN, UUID 
            FROM {$TABLE_NAME}
            WHERE   TYPE = 'plugin'
             AND    PLUGIN != 'LDAP'
             AND    ACTION = 'open'
            GROUP BY ACTION,PLUGIN, UUID
            ORDER BY CNT DESC LIMIT 5";
        $ret = sqlite_query($query, $res);

        $colSize = 16;
        $title = FALSE;
        foreach(sqlite_fetch_all($ret) as $entry){
            foreach($entry as $key => $str){
                if(is_numeric($key)) continue;
                echo str_pad($str,$colSize,' ')."|"; 
            }
            echo "\n";
        }
        echo "</pre>";
        echo sqlite_error_string($ret);
    }
}


?>