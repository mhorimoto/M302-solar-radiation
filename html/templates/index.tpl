<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <title>UECS for M302</title>
    <link href="css/main.css" rel="stylesheet" type="text/css">
  </head>
  <body id="tools">
    <div id="t-header">
      <a name="pagetop"></a>
      <h1 id="tools">M302 builder</h1>
    </div>
    <div id="wrapper">
      <h2>NODE INFO</h2>
      <form action="index.php" method="POST">
	<div class="d02 ws-padding">
	  <table class="t04">
	    <tr>
	      <th class="t-right">Node Name</th>
	      <td><input type="TEXT" name="afn" size="18" value=""></td>
	    </tr>
	    <tr>
	      <th class="t-right">Vender Name</th>
	      <td>
		<select name="ven">
		  <option value="AMPSD">AMPSD</option>
		  <option value="HOLLY">HOLLY&amp;Co.,Ltd.</option>
		  <option value="YSL">  LLC YS Lab</option>
		</select>
	      </td>
	    </tr>
	    <tr>
	      <th class="t-right">UECS-ID</th>
	      <td><input type="TEXT" name="uecsid" size="12" maxlength="12" value="01100C00000B" pattern="^[0-9A-Fa-f]*$">
		<span class="s2-color">OPTION</span></td>
	    </tr>
	    <tr>
	      <th class="t-right">MAC Address</th>
	      <td>
		<input type="TEXT" name="mac" size="17" maxlength="17" value="02:a2:73:" pattern="^[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].:[0-9A-Fa-f].$">
		<span class="s2-color">REQUIRED</span></td>
	    </tr>
	    <tr>
	      <td>&nbsp;</td>
	      <td>
		<span style="align:right;">
		  <a href="memmap.html" target="_MEMMAP">
		    <input type="button" value="HELP MEMORY MAP">
		  </a>
		</span>
	      </td>
	    </tr>
	  </table>
	</div>
	<h2>CCM INFO</h2>
	<div class="d02 ws-padding">
	  <table class="t05">
	    <tr>
	      <th rowspan="2">No.</th>
	      <th width="150">Sensor</th>
	      <th width="60">Send/Recv</th>
	      <th width="80">SR Lev</th>
	      <th>ROOM</th>
	      <th>REGION</th>
	      <th>ORDER</th>
	      <th>PRIORITY</th>
	    </tr>
	    <tr>
	      <th>CCM Type</th>
	      <th>Cast</th>
	      <th>Unit</th>
	      <th colspan="4">Device</th>
	    </tr>
	    <tr>
	      <td rowspan="2">0</td>
	      <td><br></td>
	      <input type="HIDDEN" name="SENSOR[0]" value="condition">
	      <td>Send</td>
	      <input type="HIDDEN" name="SR[0]" value="Send">
	      <td>A-1S-0</td>
	      <input type="HIDDEN" name="SRLev[0]" value="A-1S-0">
	      <td><input type="NUMBER" name="ROOM[0]" min="0" max="127" size="3" maxlength="3" value="1"></td>
	      <td><input type="NUMBER" name="REGION[0]" min="0" max="127" size="3" maxlength="3" value="1"></td>
	      <td><input type="NUMBER" name="ORD[0]" min="0" max="30000" size="5" maxlength="5" value="1"></td>
	      <td>29</td>
	      <input type="HIDDEN" name="PRIORITY[0]" value="29">
	    </tr>
	    <tr>
	      <td><input type="TEXT" name="CCMTYPE[0]" size="20" maxlength="20" value="cnd"></td>
	      <td><input type="HIDDEN" name="CAST[0]" value="0">0</td>
	      <td><input type="HIDDEN" name="UNIT[0]" value="">None</td>
	      <td colspan="4">None<input type="HIDDEN" name="DEVICE[0]" value=""></td>
	    </tr>
	    {for $idx=1 to 7}
	    <tr>
	      <td rowspan="2">{$idx}</td>
	      <td>
		<select name="SENSOR[{$idx}]">
		  <option>--</option>
		  <option>PCM-01N Solar Radiation</option>
		  <option>Temperature</option>
		  <option>Humidity</option>
		  <option>CO2</option>
		</select>
	      </td>
	      <td>
		<select name="SR[{$idx}]">
		  <option>Send</option>
		  <option>Recv</option>
		</select>
	      </td>
	      <td>
		<select name="SRLev[{$idx}]">
		  <option>A-1S-0</option>
		  <option>A-1S-1</option>
		  <option>A-10S-0</option>
		  <option>A-10S-1</option>
		  <option>A-1M-0</option>
		  <option>A-1M-1</option>
		  <option>B-0</option>
		  <option>B-1</option>
		  <option>S-1S-0</option>
		  <option>S-1M-0</option>
		</select>
	      </td>
	      <td><input type="NUMBER" name="ROOM[{$idx}]" min="0" max="127" size="3" maxlength="3" value="1"></td>
	      <td><input type="NUMBER" name="REGION[{$idx}]" min="0" max="127" size="3" maxlength="3" value="1"></td>
	      <td><input type="NUMBER" name="ORD[{$idx}]" min="0" max="30000" size="5" maxlength="5" value="1"></td>
	      <td><input type="NUMBER" name="PRIORITY[{$idx}]" min="0" max="30" size="2" maxlength="2" value="15"></td>
	    </tr>
	    <tr>
	      <td><input type="TEXT" name="CCMTYPE[{$idx}]" size="20" value=""></td>
	      <td><input type="NUMBER" name="CAST[{$idx}]" size="2" min="0" max="18" value="0"></td>
	      <td><input type="TEXT" name="UNIT[{$idx}]" size="8" placeholder="単位" value=""></td>
	      <td colspan="4">
		<select name="DEVICE[{$idx}]">
		  <option>--</option>
		  <option>SHT-2x</option>
		  <option>SHT-3x</option>
		  <option>S/D-300</option>
		  <option>S/D-400</option>
		  <option>SX8725C</option>
		</select>
	      </td>
	    </tr>
	    {/for}
	  </table>
	</div>
	<div id="m-btn">
	  <input type="SUBMIT" name="EXECMODE" value="Build">
	  <input type="SUBMIT" name="EXECMODE" value="arraydump">
	  <input type="RESET" name="EXECMODE" value="Cancel">
	</div>
      </form>
  </body>
</html>
