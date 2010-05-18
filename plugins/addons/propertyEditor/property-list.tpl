{if !$warningNotAccepted}

    <table summary="{t}Warning message{/t}" style='width:100%'>
     <tr>
      <td style='padding: 10px;'>{image path='images/warning.png'}</td>
      <td>
        <h3>Attention</h3>
       <p>
        {t}Modifying properties may break your setup, destroy or mess up your ldap database, lead to security holes or it can even make logins impossible!{/t}
        {t}Since configuration properties are stored in the ldap database a copy/backup can be handy.{/t}
       </p>
      </td> 
     </tr>
    </table>
    <hr>
      <input type='checkbox' name='warningAccepted' value='1'>
        {t}I know the risks and want to modify properties!{/t}
    <hr>
    <div class="plugin-actions">
        <button name='goOn'>{msgPool type='okButton'}</button> 
    </div>

{else}

<script language="javascript" src="include/tooltip.js" type="text/javascript"></script>

<div id="mainlist">

  <div class="mainlist-header">
   <p>{$HEADLINE}&nbsp;{$SIZELIMIT}</p>
   <div class="mainlist-nav">
    <table summary="{$HEADLINE}">
     <tr>
      <td>{$RELOAD}</td>
      <td class="left-border">{$ACTIONS}</td>
      <td class="left-border">{$FILTER}</td>
     </tr>
    </table>
   </div>
  </div>
  {$LIST}
</div>

<script type="text/javascript">
  Event.observe(window,"load",function() {
    $$("*").findAll(function(node){
      return node.getAttribute('title');
    }).each(function(node){
        var test = node.title;
      var t = new Tooltip(node,test);
      t.options.delta_x = -150;
      t.options.delta_y = -50;
      node.removeAttribute("title");
    });
  });
</script>

{if !$is_modified}
<input type="hidden" name="ignore">
{/if}

<div class="plugin-actions">
    <button name='saveProperties'>{msgPool type='okButton'}</button>
    <button name='cancelProperties'>{msgPool type='cancelButton'}</button>
</div>
{/if}