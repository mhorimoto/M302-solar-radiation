<?php

function mk4hexdump($v,$c) {
    // $v input string
    // $c byte count
    $rv = "";
    for($i=0;$i<$c;$i++) {
        $hv .= substr($v,$i*2,2)." ";
    }
    $hv = rtrim($hv);
    return($hv);
}

function mk4ccmtbl($a,$p) {
    // $a: array $AAA
    // $p: index position ($p=0 is cnd)
    // Return values are 2 line
    // byte position
    // 00: enable=1/disable!=1
    // 01: room
    // 02: region
    // 03-04: order (LSB-MSB)
    // 05: priority
    // 06-25: ccm type name
    // 26: itvl Interval
    // itvl[0:1]=00=受信専用CCMのため送信しない
    //           01=A
    //           10=B
    //	         11=S
    // itvl[2:6]=00001=1S
    //           00010=10S
    //	         00100=1M
    // itvl[7]  =0 値変化時に送信しない
    //           1 値変化時に送信する
    // 27-31: 予約
    $sensor  = $a['SENSOR'][$p];
    $ccmtype = $a['CCMTYPE'][$p];
    $sr      = $a['SR'][$p];
    $srlev   = $a['SRLev'][$p];
    $room    = $a['ROOM'][$p];
    $region  = $a['REGION'][$p];
    $order   = $a['ORD'][$p];
    $priority= $a['PRIORITY'][$p];
    $cast    = $a['CAST'][$p];
    $unit    = $a['UNIT'][$p];
    $device  = $a['DEVICE'][$p];
    $tCCMTYPE  = substr($ccmtype,0,20);
    if ($p==0) { // cnd type force
        $txt = "01";  // Force enabled
    } else {
        if ($tCCMTYPE) {
            $txt = "01";
        } else {
            $txt = "00".str_repeat("FF",31);
            return($txt);
        }
    }
    $tCCMTYPE = chr2hex($tCCMTYPE,20);
    $tROOM = sprintf("%02X",intval($room,10));
    $tREGION = sprintf("%02X",intval($region,10));
    $vorder = intval($order,10);
    $vorder_msb = ($vorder>>8)&0xff;
    $vorder_lsb = $vorder&0xff;
    $tORDER = sprintf("%02X%02X",$vorder_lsb,$vorder_msb);
    $tPRIORITY = sprintf("%02X",intval($priority,10));
    $tSRLev    = sprintf("%02X",srlev2code($srlev));
    $txt .= $tROOM.$tREGION.$tORDER.$tPRIORITY.$tCCMTYPE.$tSRLev.str_repeat("FF",5);
    return($txt);
}

function srlev2code($itv) {
    switch($itv) {
    case "A-1S-0":
        $rv = 0b01000010;
        break;
    case "A-1S-1":
        $rv = 0b01000011;
        break;
    case "A-10S-0":
        $rv = 0b01000100;
        break;
    case "A-10S-1":
        $rv = 0b01000101;
        break;
    case "A-1M-0":
        $rv = 0b01001000;
        break;
    case "A-1M-1":
        $rv = 0b01001000;
        break;
    case "B-0":
        $rv = 0b10000000;
        break;
    case "B-1":
        $rv = 0b10000001;
        break;
    case "S-1S-0":
        $rv = 0b11000010;
        break;
    case "S-1M-0":
        $rv = 0b11001000;
        break;
    default:
        $rv = 0;
    }
    return($rv);
}


function uecsid2hex($it) {
    if (strlen($it)!=12) {
        return("000000000000");
    } else {
        return(strtoupper($it));
    }
}

function hex2hex($it,$cnt) {
    $array_it = str_sprint($it);
}

function mac2hex($it) {
    $maca = str_replace(":","",$it);
    return(strtoupper($maca));
}

function chr2hex($it,$cnt) {
    $array_it = str_split($it);
    $hchr = "";
    foreach($array_it as $ai) {
        $hchr .= sprintf("%02X",ord($ai));
    }
    $hchr .= str_repeat("00",$cnt);
    $hchr = substr($hchr,0,$cnt*2);
    return($hchr);
}

function hex2chr($it,$cnt) {
    $md = str_replace(" ","",$it);
    $txt = "";
    for ($i=0;$i<$cnt;$i++) {
        $c = substr($md,$i*2,2);
        $d = intval($c,16);
        if (($d<0x20)||($d>0x7f)) {
            $txt .= '.';
        } else {
            $txt .= chr($d);
        }
    }
    return($txt);
}

function mk4eepromimage($it,$cnt,$sa) {
    // $it:  data (made by mk4hexdump)
    // $cnt: data count
    // $sa:  start address
    $txt = "";
    $dt = explode(" ",$it);
    for ($i=0;$i<$cnt;$i++) {
        $txt .= sprintf("setb %x %s\n",$sa,$dt[$i]);
        $sa++;
    }
    return($txt);
}

?>
