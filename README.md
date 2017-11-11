# c_lib_lstring
通过内存池句柄释放的 C 语言字符串极简实现

C++ 用久了会觉得好累,还是 C 单纯明了. 但纯 C 最大的问题的内存释放,每次都要 free 实在不烦其烦. 而且纯 C 又没有函数退出时自动调用析构函数的机制,
所以一直也没转到纯 C 上来. 一日在写 delphi 程序的时候忽然想起 delphi 的窗体释放也是自动的啊,仅仅多加一个父级窗体的句柄就能完成了. OK. 把这个
父级释放子控件的思想换到 C 的内容释放上来,一切都顺风顺水了. 我后来听说 nginx 也有类似的内存池机制,也算英雄所见略同,当然了这两个英雄是 delphi 和
nginx ,不是我. 不过为了不引起太大的混乱,还要引入一下 php 源码对内存等资源的释放思想: 那就是所有内存在一次 php 脚本解析运行终了后全部统一释放.

有了这两种思想,写作中不再为内存烦恼了,实际工程中完成过 md5,des 等比较复杂的过程,抱着这两种思想为基础很容易的就实现了自释放的版本. 从此这个小小的
lstring.c 文件就成为了我必不可少的工具. 因为工作环境的一再变迁 lstring 的版本有了不同的差异,所以我觉得还是放到 github 上来,以免自己都搞混乱了.
如果网友们要使用不要被它的源码吓倒,以后我会传些实现的调用例子上来,大家可以看到是很简单的.用过 delphi 的同学就更是觉得亲切了 :)

这个项目的核心文件只有两个 lstring.c 和 lstring_functions.c , 其中前者基本上不会再修改,后者也基本上是 C 函数的封装而已. 
另外因为本人写的程序网络应用为主,因此常用 socketplus.c 来在 windows 写测试客户端,所以有类似需求的同学们也可以看看.
