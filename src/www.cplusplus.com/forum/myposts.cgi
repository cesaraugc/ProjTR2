9d5
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
     "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title>User sign in: This feature is only available for registered users</title>
<style type="text/css">
<!--
BODY, TABLE, INPUT {font-family:verdana,arial,helvetica,sans-serif; font-size:12px; color:black; }
BODY {background-color:white; padding-top:100px; width:100%}
.tit {text-align:center; font-size:large; margin:5px;}
.main {width:720px; margin:auto;}
.libox {border:1px solid black; margin:auto;}
.libox TH {font-size:large; color:#FFFFFF; background:blue url("/img/blue.gif"); }
.libox .lileft {background: #E0E0FF}
.libox .lileft .subform {border:1px solid #000080; margin:15px}
.libox .lileft .field {text-align:right}
.libox .liright {background: #F0F0FF; text-align:center; vertical-align:top}
-->
</style>
</head>

<body>
<div class="main">
<div class="tit">This feature is only available for registered users</div><table class="libox">
<tr><th colspan="2">Sign in</th></tr>
<tr><td class="lileft">
<form action="/user/access.cgi" name="loginform" method="post">
<input type="hidden" name="w" value="login">
<input type="hidden" name="y" value="1">
<input type="hidden" name="to" value="/forum/myposts.cgi">
Sign in using your <b>cplusplus.com</b> account:
<table class="subform">
<tr class="odd"><td class="field"><b>User name:</b></td><td class="value"><input name="l" size="30" type="text" value="" class="txt"></td></tr>
<tr class="even"><td class="field"><b>Password:</b></td><td class="value"><input name="p" size="30" type="password" class="txt"></td></tr>
<tr class="odd"><td class="field"><b>Remember me:</b></td><td class="description"><input type="checkbox" name="rememberme" value="1"  > (keeps you logged in between sessions)</td></tr>
<tr class="even"><td colspan="2" align="center" class="buttons"><input type="submit" value="submit" class="btn"></td></tr>
</table>
</form>
</td>
<td class="liright">
Or, using an external account:<br>
<p>
<a href="/user/access.cgi?w=login&pr=google&to=%2Fforum%2Fmyposts.cgi&y=1">
<img src="/ico/login_google.png" width="70" height="30">
</a>
</p>
<p>
<a href="/user/access.cgi?w=login&pr=yahoo&to=%2Fforum%2Fmyposts.cgi&y=1">
<img src="/ico/login_yahoo.png" width="70" height="30">
</a>
</p>
</td>
</tr>
</table>

<div style="text-align:center; margin:10px;">
[<a href="/user/access.cgi?w=lostpass">I have lost my password</a>]
&nbsp; &nbsp;
[<a href="/user/access.cgi?w=signup">Create a new account</a>]
</div>

</div>

<p>

</body>
</html>

0

