<input type="hidden" name="mailedit" value="1">
<table summary="" style="width:100%; vertical-align:top; text-align:left;" cellpadding=0 border=0>

 <!-- Headline container -->
 <tr>
  <td style="width:50%; vertical-align:top;">
   <h2><img alt="" align="middle" src="images/rightarrow.png"> {t}Mail distribution list{/t}</h2>
   <table summary="">
    <tr>
     <td>{t}Primary address{/t}{$must}</td>
     <td><input name="mail" size=25 maxlength=65 {$mailACL} value="{$mail}" title="{t}Primary mail address for this distribution list{/t}"></td>
    </tr>
   </table>
  </td>
 </tr>
</table>

<!-- Place cursor -->
<script language="JavaScript" type="text/javascript">
  <!-- // First input field on page
  document.mainform.mail.focus();
  -->
</script>
