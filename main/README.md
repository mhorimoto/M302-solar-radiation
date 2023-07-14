# Main Program

## UECS IDなどを記録するメモリマップ (IDmap)

0x000〜0x07f

アドレス
0x000〜0x005: UECS-ID
0x006〜0x00b: MACアドレス
0x00c: 0xff=DHCP
       0x00=固定IP
0x00d〜0x00f: 予約(0xff)
0x010〜0x013: IPアドレス (固定IPの場合)
0x014〜0x017: Subnet mask アドレス (固定IPの場合)
0x018〜0x01b: Default Gateway アドレス (固定IPの場合)
0x01c〜0x01f: DNSサーバアドレス (固定IPの場合)
0x040〜0x04f: VENDER名
0x050〜0x05f: NODE名

## CCM typeを記録するメモリマップ (CCMmap1)

0x080〜0x17f
32bytes/1record
00=cnd

byte位置 
00: enable=1/disable!=1
01: room
02: region
03-04: order (low-highの順)
05: priority
06-25: ccm type名
26: itvl インターバル
    itvl[0:1]=00=受信専用CCMのため送信しない
              01=A
              10=B
	      11=S
    itvl[2:6]=00001=1S
              00010=10S
	      00100=1M
    itvl[7]  =0 値変化時に送信しない
              1 値変化時に送信する
27-31: 予約
