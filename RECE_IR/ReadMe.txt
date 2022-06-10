Keil C51中的printf()与标准的C库的printf()函数稍有不同，在相应的帮助文档中有如下描述：
The optional characters l or L may immediately precede the type character to respectively specify long types for d, i, u, o, x, and X.
The optional characters b or B may immediately precede the type character to respectively specify char types for d, i, u, o, x, and X.
也就是说，使用C51的printf()函数打印%d/i/u/o/x/X格式时，你必须要指定该变量的存储格式l/L/b/B。
由于你的变量c为char类型，因此可将相应的代码改为如下：
printf("%bd\n",c);
printf("%bu\n",c);
printf("%bx\n",c);
即可得到正确的结果。
另：
若变量c为uint16时，则需将%bd等改为%hd；(这个是C51帮助文档里面没有的，后来自己总结出来的)
若变量c为uint32时，则需将%bd等改为%ld；
希望能帮助到你。


打印出来的c%是在内存里面取的是一个字节的值,没有问题,但是一旦打印%d,%y,%x这样的变量的时候,在内存里面取的是以c的变量存储地址为起始地址,
以两个字节为数据单元,取出来的值,然后又因为c为地址的后一个地址单元的数据是0,所以,取出来的值是256的倍数.