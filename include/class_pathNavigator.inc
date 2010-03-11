<?php

class pathNavigator
{

  static function registerPlugin($class)
  {
    $str = "";
    if($class instanceOf management && isset($class->plHeadline)){
      $str= _($class->plHeadline);
    }elseif($class instanceOf plugin && isset($class->plHeadline)){
#      $str = _($class->plHeadline);
    }elseif($class instanceOf plugin && !isset($class->plHeadline)){
      $str = "{".get_class($class)."}";
    }elseif($class instanceOf tabs){
      $str = $class->dn;
    }elseif(is_string($class)){
      $str = $class;
    }
    
    if(!empty($str)){
      $cur = session::get('pathNavigator::position');
      if(empty($cur)){
        $cur.= "<li class='left path-element'>{$str}</li>";
      }else{
        $cur.= "<li class='left path-element'>{$str}</li>";
      }
      session::set('pathNavigator::position', $cur);
    }
  }

  static function getCurrentPath()
  {
    $path = session::get('pathNavigator::position');
    if(empty($path)){
      $path = "<li class='left path-element'>"._("Welcome to GOsa")."</li>";
    }
    return($path);
  }

  static function clear()
  {
    session::set('pathNavigator::position','');
  }
}

// vim:tabstop=2:expandtab:shiftwidth=2:filetype=php:syntax:ruler:
?>