<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <title>HEXDUMP UECS for M302</title>
    <link href="css/main.css" rel="stylesheet" type="text/css">
  </head>
  <body id="tools">
    <div id="t-header">
      <a name="pagetop"></a>
      <h1 id="tools">Hex dump output for M302 builder</h1>
    </div>
    <div id="wrapper">
      <h2>MEMORY MAP HEX DUMP</h2>
      <div class="d02 ws-padding">
	<table class="t04">
	  <tr>
	    <td valign="top">
	      <pre>
               |00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F| 0123456789ABCDEF
          -----+-----------------------------------------------+------------------
          0x000|{$hUECSID} {$hMACA} FF FF FF FF| {$tUECSID}{$tMACA}....
	  0x010|FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF| ................
	  0x020|{$hVEN}| {$tVEN}
	  0x030|{$hAFN}| {$tAFN}
	  0x040|FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF| ................
	  0x050|FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF| ................
	  0x060|FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF| ................
	  0x070|FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF| ................
	  0x080|{$hCCML0}| {$tCCML0}
	  0x090|{$hCCMH0}| {$tCCMH0}
	  0x0A0|{$hCCML1}| {$tCCML1}
	  0x0B0|{$hCCMH1}| {$tCCMH1}
	  0x0C0|{$hCCML2}| {$tCCML2}
	  0x0D0|{$hCCMH2}| {$tCCMH2}
	  0x0E0|{$hCCML3}| {$tCCML3}
	  0x0F0|{$hCCMH3}| {$tCCMH3}
	  0x100|{$hCCML4}| {$tCCML4}
	  0x110|{$hCCMH4}| {$tCCMH4}
	  0x120|{$hCCML5}| {$tCCML5}
	  0x130|{$hCCMH5}| {$tCCMH5}
	  0x140|{$hCCML6}| {$tCCML6}
	  0x150|{$hCCMH6}| {$tCCMH6}
	  0x160|{$hCCML7}| {$tCCML7}
	  0x170|{$hCCMH7}| {$tCCMH7}
	      </pre>
	    </td>
	    <td>
	      <p>EEPROM DATA</p>
	      <pre>
{$eROM}
	      </pre>
	    </td>
	  </tr>
	</table>
      </div>
      <a href="index.php">INPUT</a><br>
    </div>
  </body>
</html>
