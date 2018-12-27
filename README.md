## RoboSys2018 HW1

#Rock-scissors-paper

じゃんけんで勝つとLEDが光る  

## Usage
じゃんけんをする方法
/dev/myled0 にr(Rock), s(Scissors), p(Paper) のいずれかを送信  
ex)グーを出す場合
```
echo r > /dev/myled0
```

出す手の確認方法
```
cat /dev/myled0
```
Rock	->	r  
Scissors	-> s  
Paper	->	p  

CPの出した手の確認方法(CPがチョキを出した場合)
```
tail /var/log/messages
```
cp_hand = Scissors  

##Install
insmod myled.ko  

##LICENCE
GNU GPL
