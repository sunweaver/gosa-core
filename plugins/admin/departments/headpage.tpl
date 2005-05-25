<table style="width:100%; vertical-align:top; text-align:left;" cellpadding=4>
<tr>
  <td style="vertical-align:top;">
  <div class="contentboxh">
    <p class="contentboxh">
     {t}List of departments{/t} {$hint}
    </p>
  </div>
  <div class="contentboxb">
    <p class="contentboxb">
     <select style="width:500px; height:450px;" name="deplist" size=15 ondblclick="this.form.edit_helper.value='1'; this.form.submit()">
      {html_options options=$departments}
     </select>
    <br>
    <input type=submit value="{t}New{/t}..." name="new_department">&nbsp;
    <input type=submit value="{t}Edit{/t}" name="select_department">&nbsp;
    <input type=submit value="{t}Delete{/t}" name="delete_department">&nbsp;
    <input type=hidden name="edit_helper">
   </p>
  </div>
  </td>
  <td style="vertical-align:top;">
   <div class="contentboxh">
    <p class="contentboxh"><img src="{$infoimage}" align="right" alt="[i]">{t}Information{/t}</p>
   </div>
   <div class="contentboxb">
   <p class="contentboxb">
     {t}This menu allows you to create, delete and edit selected departments. Having a large size of departments, you might prefer the range selectors on top of the department list.{/t}
   </p>
   <p class="contentboxb">
     {t}-Edit- and -New...- will provide an assistant to aid you when performing changes on your departments. -Delete- will ask for confirmation before deleting departments.{/t}
   </p>
   </div>
   <br>
   <div class="contentboxh">
    <p class="contentboxh"><img src="{$launchimage}" align="right" alt="[F]">{t}Filters{/t}</p>
   </div>
   <div class="contentboxb">
     <table style="width:100%;border-top:1px solid #B0B0B0;">
      {$alphabet}
     </table>
    <table  style="width:100%;border-top:1px solid #B0B0B0;">
			<tr>
					<td width="50%"><img alt="" src="{$tree_image}" align=middle>&nbsp;{t}Display subdepartments of{/t}</td>
				    <td>
						<select name="depselect" size=1 onChange="mainform.submit()" title="{t}Choose the department the search will be based on{/t}">
						      {html_options options=$deplist selected=$depselect}
					    </select>
					</td>
			</tr>
	</table>
    <table  style="width:100%;border-top:1px solid #B0B0B0;">
			<tr>
				<td width="50%">
						<img alt="" src="{$search_image}" align=middle>&nbsp;{t}Display departments matching{/t}</td>
			    <td>
						<input type='text' name='regex' maxlength='20' value='{$regex}' 
							title='{t}Regular expression for matching department names{/t}' onChange="mainform.submit()">
				</td>
			</tr>
	</table>
			    {$apply}
   </div>
  </td>
</tr>
</table>

<input type="hidden" name="ignore">
