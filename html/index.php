<?php
require_once('Smarty.class.php');
require_once('ope_ccmtbl.php');

$s = new Smarty();
$EM=$_POST["EXECMODE"];
$AFN=$_POST["afn"];
$VEN=$_POST["ven"];
$UECSID=$_POST["uecsid"];
$MACA=$_POST["mac"];
$AAA=$_POST;
if ($EM=="Build") {
    $eepromtxt = "";
    $v  = uecsid2hex($UECSID);
    $hv = mk4hexdump($v,6);
    $tv = hex2chr($hv,6);
    $ep = mk4eepromimage($hv,6,0);
    $s->assign("hUECSID",$hv);
    $s->assign("tUECSID",$tv);
    $v = mac2hex($MACA);
    $hv = mk4hexdump($v,6);
    $tv = hex2chr($hv,6);
    $ep .= mk4eepromimage($hv,6,6);
    $s->assign("hMACA",$hv);
    $s->assign("tMACA",$tv);
    $v = chr2hex($VEN,16);
    $hv = mk4hexdump($v,16);
    $tv = hex2chr($hv,16);
    $ep .= mk4eepromimage($hv,16,0x20);
    $s->assign("hVEN",$hv);
    $s->assign("tVEN",$tv);
    $v = chr2hex($AFN,16);
    $hv = mk4hexdump($v,16);
    $tv = hex2chr($hv,16);
    $ep .= mk4eepromimage($hv,16,0x30);
    $s->assign("hAFN",$hv);
    $s->assign("tAFN",$tv);
    for ($i=0;$i<8;$i++) {
        $ccmtbl=mk4ccmtbl($AAA,$i);
        $hv = mk4hexdump($ccmtbl,16);
        $tv = hex2chr($hv,16);
        if (substr($ccmtbl,0,2)=="01") {
            $ep .= mk4eepromimage($hv,16,0x80+(0x10*$i*2));
        }
        $s->assign("hCCML$i",$hv);
        $s->assign("tCCML$i",$tv);
        $hv = mk4hexdump(substr($ccmtbl,32,32),16);
        $tv = hex2chr($hv,16);
        if (substr($ccmtbl,0,2)=="01") {
            $ep .= mk4eepromimage($hv,16,0x90+(0x10*$i*2));
        }
        $s->assign("hCCMH$i",$hv);
        $s->assign("tCCMH$i",$tv);
    }
    $s->assign("eROM",$ep);
    $s->display("hexdump.tpl");
    exit;
} else if ($EM=="arraydump") {
    echo "<pre>\n";
    print_r($AAA);
    echo "</pre>\n";
    exit;
}
$s->display("index.tpl");
exit;


?>
