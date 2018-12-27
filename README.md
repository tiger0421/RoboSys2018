## RoboSys2018
##Rock-scissors-paper
じゃんけんで勝つとLEDが光る

## Usage
じゃんけんをする方法
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

グー	->	r
チョキ	->	s
パー	->	p

ex)グーを出す場合
```
echo r > /dev/myled0
```

CPの出した手の確認方法(CPがチョキを出した場合)
```
tail /var/log/messages
```
cp_hand = Scissors

##Install
insmod myled.ko

##LICENCE
GNU GPL
