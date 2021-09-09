#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>

//动态内存管理

//本章重点
//为什么存在动态内存分配
//动态内存函数的介绍
   //malloc
   //free
   //calloc
   //realloc
//常见的动态内存错误
//几个经典的笔试题
//柔性数组





//为什么存在动态内存分配

//我们已经掌握的内存开辟的方式有：

//int val =20;//在栈空间上开辟四个字节
//char arr[10]={0};//在栈空间上开辟10个字节的连续空间

//但是上述的开辟空间的方式有两个特点：

//1、空间开辟大小是固定的
//2、数组在申明的时候必须指定数组的长度，它所需要的内存在编译时分配

//但是对于空间的需求，不仅仅时上述的情况。有时候我们需要的空间大小在程序运行的时候才能知道，
//那数组的编译时开辟空间的方式就不能满足了，这时候就只能试试动态存开辟了

//内存块会分为三个部分，从高到低，依次为：栈区，堆区，静态区
//栈区的作用：局部变量，函数的形式参数
//堆区的作用：动态内存分配
//静态区的作用：全局变量，静态变量

//这时会有下面两种情况：
//1、创建一个变量：
//int a = 10;//局部变量 - 会放置到栈区
//int g_a = 10;//全局变量 - 会放置到静态区中

//由此可知 
//当这个变量是局部变量的时候，系统就会自动放到栈区
//当这个变量是全局变量的时候，系统就会自动放到静态区

//2、创建一个数组的时候
//同样，若是局部变量，则就会放置到栈区，若为全局变量，则会放置到静态区


//下面的这个例子：

//struct S
//{
//	char name[20];
//	int age;
//};
//int main()
//{
//	int n = 0;
//	scanf("%d", &n);
//	struct S arr[n];//假如需要50个struct S 类型的空间，如果给了30，则不够，若给了60，则又多
//	//这就是我需要多少，系统就给我开辟多少个字节的空间
//	//在上面可以知道，数组后面是不能写变量的，只能写成常量。
//	return 0;
//}

//在C99标准下，数组后面可以放置成变量，所以gcc编译器是支持C99标准的
//但大部分编译器是不支持这一条规定的



//于是为了满足动态内存分配，出现了动态内存函数
//动态内存函数的介绍

//malloc和free

//malloc (Allocates memory blocks.)

//void *malloc( size_t size );

//这里需要注意的是malloc的返回类型是void*

//这个函数向内存申请一块连续可用的空间，并返回指向这块空间的指针

  //如果开辟成功，则返回一个指向开辟好空间的指针
  //如果开辟失败，则返回一个NULL指针，因此malloc的返回值一定要做检查
  //返回值的类型是void* ，所以malloc函数并不知道开辟空间的类型，具体在使用的时候使用者自己来决定
  //如果参数size 为0，malloc行为的标准是未定义的，取决于编译器



//C语言还提供了另外一个函数free ,专门是用来做动态内存的释放和回收，函数原型如下：
//free (Deallocates or frees a memory block.)

//void free( void *memblock );

//这里这个返回类型是void 

//free函数用来释放动态开辟的内存。
  //如果参数 ptr 指向的空间不是动态开辟的，那free函数的行为是未定义的。
  //如果参数 ptr 是NULL指针，则函数什么事都不做。
  //malloc和free都声明在 stdlib.h 头文件中。

////例子：
//#include <stdlib.h>
//#include <errno.h>
//int main()
//{
//	//向内存申请10个整型的空间
//	//int* p = malloc(10 * sizeof(int));//因为这开辟的是空间，所以要把地址存到int* 的类型中
//	//在申请这块空间的地址时，malloc的返回类型是void* 类型，而p的类型是int*，所以要对
//	//malloc进行强制类型转换
//	
//	//int* p = (int*)malloc(10 * (sizeof(int)));
//	//int* p = (int*)malloc(INT_MAX);//INT_MAX是一个极大的数，这个可以转到定义查看这个数字
//	int* p = (int*)malloc(40);
//	//这下要考虑malloc开辟空间失败的情况
//	if (p == NULL)//如果失败，则返回空指针
//	{
//		//打印错误原因的一种方式
//		printf("%s\n", strerror(errno));
//	}
//	else
//	{
//		//正常使用空间
//		int i = 0;
//		for (i = 0; i < 10; i++)
//		{
//			*(p + i) = i;
//		}
//		for (i = 0; i < 10; i++)
//		{
//			printf("%d ", *(p + i));
//		}
//	}
//	//当动态申请的空间不再使用的时候
//	//就应该还给操作系统
//	free(p);
//	p = NULL;
//	return 0;
//}


//只有当malloc开辟空间的时候才需要free释放
//数组存放是不需要free释放空间的



////上面这个也可以用calloc来写
//#include <errno.h>
//#include <stdlib.h>
//int main()
//{
//	//malloc(10*sizeod(int))
//	int* p = (int*)calloc(10, sizeof(int));//这个是开辟一段空间，并且进行初始化，
//	//malloc开辟空间，但不进行初始化
//	if (p == NULL)
//	{
//		printf("%s\n", strerror(errno));
//	}
//	else
//	{
//		int i = 0;
//		for (i = 0; i < 10; i++)
//		{
//			printf("%d ", *(p + i));
//		}
//	}
//	return 0;
//}

//所以如果想要初始化空间，就用calloc
//    如果不想进行初始化空间，则就用malloc


//calloc (Allocates an array in memory with elements initialized to 0.)

//void *calloc( size_t num, size_t size );

//注意，这个返回类型是void* 所以接收是也要用指针来接收

//函数功能是为num个大小为size的元素开辟一块空间，并且把空间的每个字节都初始化为0
//与函数malloc的区别只在于calloc会在返回地址之前把申请的空间的每个字节初始化为全0
//例子在上面


//realloc (Reallocate memory blocks.) -(重新分配内存块。)
//调整动态开辟内存空间的大小

//realloc函数的出现让动态内存管理更加灵活
//有时会我们发现过去申请的空间太小了，有时候我们又会觉得申请的空间过大了，那为了合理的使用内存
//我们一定会对内存的大小做灵活的调整。函数原型如下：

//void *realloc( void *memblock, size_t size );

//返回类型依然是void* 

  //memblock是要调整的内存地址
  //size调整之后的新大小
  //返回值为调整之后的内存起始位置
  //这个函数调整原内存空间大小的基础上，还会将原来内存中的数据移动 新 的空间
  //realloc在调整内存空间的是存在两种情况：
     //1、原有空间之后有足够大的空间
     //2、原有空间之后没有足够大的空间





//#include <errno.h>
//#include <stdlib.h>
//int main()
//{
//	int* p = (int*)malloc(20);
//	if (p == NULL)
//	{
//		printf("%s\n", strerror(errno));
//	}
//	else
//	{
//		int i = 0;
//		for (i = 0; i < 5; i++)
//		{
//			*(p + i) = i;
//		}
//	}
//	//就是在使用malloc开辟的20个字节空间
//	//假设这里，20个字节不能满足我们的使用了
//	//希望我们能够有40个字节的空间
//	//这里就可以使用realloc来调整动态开辟的内存
//
//	//int* p2 = realloc(p, 40);
//	//p = realloc(p, 40);//这样子写出来的代码，编译出来没问题，但风险极大
//	////realloc后面是20的时候，由于内存开辟不足，所以就会导致系统崩溃
//	//p = realloc(p, INT_MAX);//如果需要的空间太大，导致开辟失败，则就会返回空指针
//	//这样的问题在于放入p中，会导致p的空间开辟失败且将原来存储的值同样弄销毁，也就是将旧的数据销毁
//
//	//realloc的注意事项：
//  //1、如果p指向的空间之后有足够的内存空间可以追加，则直接追加，之后返回p
//  //2、如果p指向的空间之后没有足够的内存空间可以追加，则realloc函数会重新找一个新的内存区域
////开辟一块满足需求的空间，并且把原来内存中的数据拷贝回来，释放旧的内存空间
////最后返回新开辟的内存空间地址
//  //3、得用一个新的变量来接受realloc函数的返回值
//
//	int* ptr = realloc(p, INT_MAX);
//
//	if (ptr != NULL)
//	{
//		p = ptr;//这样子就依旧是用原来的p来维护新的内存空间
//	}
//	int i = 0;
//	for (i = 5; i < 10; i++)
//	{
//		*(p + i) = i;
//	}
//	for (i = 0; i < 10; i++)
//	{
//		printf("%d ", *(p + i));
//	}
//	return 0;
//}



//情况1
// 当是情况1的时候，要扩展内存就直接原有内存之后直接追加空间，原来空间的数据不发生变化。
//情况2
//当是情况2的时候，原有空间之后没有足够多的空间时，扩展的方法是：在堆空间上另找一个合适大小的
//连续空间来使用，这样子函数返回的是一个新的内存地址。
//由于上述的两种情况，realloc函数在使用就要注意一些


//常见的动态内存错误
////1、对NULL指针的解引用操作：
//int main()
//{
//    int* p = (int*)malloc(40);
//    //万一malloc失败了，p就被赋值为NULL
//    *p = 0;//err
//    int i = 0;
//    for (i = 0; i < 10; i++)
//    {
//        *(p + i) = i;//err
//    }
//    free(p);
//    p = NULL;
//    return 0;
//}



////2、对动态开辟的内存的越界访问
//int main()
//{
//    int* p = (int*)malloc(5 * sizeof(int));
//    if (p == NULL)
//    {
//        return 0;
//    }
//    else
//    {
//        int i = 0;
//        for (i = 0; i < 10; i++)//上面开辟了5个int大小的内存，而这里却要10个int类型的大小
//            //所以造成了越界访问
//        {
//            *(p + i) = i;
//        }
//    }
//    free(p);
//    p = NULL;
//    return 0;
//}



////3、对非动态开辟内存的free
//int main()
//{
//    int a = 10;
//    int* p = &a;
//    *p = 20;
//    free(p);//这里没用malloc所以进行了非法开辟动态内存
//    p = NULL;
//    return 0;
//}




////realloc可以实现malloc 的功能
//int main()
//{
//    int* p = (int*)malloc(40);
//    if (p != NULL)
//    {
//        return 0;
//    }
//    //使用
//    int* p2 = realloc(p, 80);
//    if (p2 != NULL)
//    {
//        p = p2;
//    }
//    return 0;
//}

//int* p = realloc(NULL, 40);//这个等于malloc(40)//前面的NULL为空指针



//4、使用free释放动态开辟内存的一部分

//int main()
//{
//    int* p = (int*)malloc(40);
//    if (p == NULL)
//    {
//        return 0;
//    }
//    int i = 0;
//    for (i = 0; i < 10; i++)
//    {
//        *p++ = i;//这个问题是p一直再改变,而释放p的地址在p指向地址的后面，而后面又什么都没有
//        //所以开辟失败
//    }
//    //运行到这里的p已经不是开辟空间的起始位置的地址了
//    //回收空间
//    free(p);
//    p = NULL;
//    return 0;
//}


////5、对同一块动态内存的多次释放
//#include <stdlib.h>
//int main()
//{
//    int* p = (int*)malloc(40);
//    if (p == NULL)
//    {
//        return 0;
//    }
//    //使用
//    //...
//    //释放
//    free(p);
//    //...
//    //若修改就加上下面这个
//    p = NULL;//加上这个就可以避免系统出现崩溃了
//    free(p);
//    return 0;
//}


////6、动态开辟内存忘记释放（内存泄漏）
//int main()
//{
//    while (1)
//    {
//        malloc(1);
//    }
//    //这样的代码，就会一直吃内存，就一直开辟空间，而没有释放空间的，所以就会一直吃掉空间
//        return 0;
//}
////还有这种的：
//void test()
//{
//    int* p = (int*)malloc(200);
//    if (p != NULL)
//    {
//        *p = 20;
//    }
//}
//int main()
//{
//    test();
//    while(1);
//    return 0;
//}

//这种都是动态内存开辟好之后就忘了释放，导致内存一直被吃，一直被消耗




////几个经典的笔试题

//1、

//void GetMemory(char* p)//这个p是str的一个临时拷贝，用完之后，就会直接被销毁
//{
//    p = (char*)malloc(100);
//}
//void Test(void)
//{
//    char* str = NULL;
//    GetMemory(str);
//    strcpy(str, "hello world");
//    printf(str);
//}
//int main()
//{
//    Test();
//    return 0;
//}
//上面这串代码的问题有哪些：

//1、运行代码程序会出现崩溃iade现象
//2、程序存在内存泄漏的问题
  //str以值传递的形式给p,p是GerMemory函数的形参，只能函数内部有效，等GetMemory函数返回之后，
  //动态开辟内存尚未释放，并且无法找到，所以会造成内存泄漏的问题

//当p用了之后，从函数内部出来的时候，就会被销毁，所以在进行传参的时候，就会出现传的值是空的
//导致问题存在


////改正1：
//void GetMemory(char** p)//这个p是str的一个临时拷贝，用完之后，就会直接被销毁
//{
//   *p = (char*)malloc(100);
//}
//void Test(void)
//{
//    char* str = NULL;
//    GetMemory(&str);//传的是地址，就要用指针来接收
//    strcpy(str, "hello world");
//    printf(str);
//
//    free(str);
//    str = NULL;
//}
//int main()
//{
//    Test();
//    return 0;
//}
////这样就没有什么问题了

////改正2
//#include <stdlib.h>
//#include <string.h>
//char* GetMemory(char* p)//这个p是str的一个临时拷贝，用完之后，就会直接被销毁
//{
//    p = (char*)malloc(100);
//    return p;//这样子的话，就不会让值销毁了
//}
//void Test(void)
//{
//    char* str = NULL;
//    str = GetMemory(str);//问题出现到了这里，如果不将GetMemory(str)的值传给str
//    //也就是没有str = ，则就会出现问题，strcpy中的str就没有什么值，所以，字符串复制的时候就会
//    //出现问题。
//    strcpy(str, "hello world");
//    printf(str);
//    free(str);
//    str = NULL;
//}
//int main()
//{
//    Test();
//    return 0;
//}



//2、

//char* GetMemory(void)
//{
//    char p[] = "hello world";
//    return p;//这里返回p的地址，之后，就直接销毁了p的里面的内容
//}
//void Test(void)
//{
//    char* str = NULL;
//    str = GetMemory();
//    printf(str);
//}
//int main()
//{
//    Test();
//    return 0;
//}
////上面这个是非法访问，虽然str指向p但p已经被销毁

//上面这个是返回栈空间的地址的问题

//
//int* test()
//{
//    static int a = 10;//这样子就将int地址放在了静态区，这样下面就可以进行访问
//    //static关键字是进行的是将内存地址放入静态区之中，这样就可以保证不被销毁，而且访问时，还存在
//    //int a = 10;//这里是栈区，下面就进行了非法访问
//    return &a;
//}
//int main()
//{
//    int* p = test();
//    *p = 20;//这就属于非法访问栈区上的内存
//    return 0;
//}

//int* test()
//{
//    int* ptr = malloc(100);//这里是将这块内存放置到了堆区
//    return ptr;
//}
//int main()
//{
//    int* p = test();
//    //这里没有free来释放这块空间，所以这块地址依旧是存在的，所以进行访问时是成功的
//    return 0;
//}



//int* f2(void)
//{
//    int* ptr;
//    *ptr = 10;
//    return ptr;//这里的问题在于ptr没有进行初始化，所以ptr里面就是随机值
//    //*ptr就造成了非法访问，其实也就是野指针的问题
//}




//int* f1(void)
//{
//    int x = 10;
//    return (&x);//这就是返回栈空间地址的问题
//    //*f1指向的&x是用完之后直接被销毁的，所以*f1造成了非法访问
//}




//3、

//void GetMemory(void** p,int num)
//{
//    *p = (char*)malloc(num);
//}
//void Test(void)
//{
//    char* str = NULL;
//    GetMemory(&str, 100);
//    strcpy(str, "hello world");
//    printf(str);//忘记释放动态开辟的内存，导致内存泄露了
//}
//int main()
//{
//    Test();
//    return 0;
//}
//这些都是可以编译出来的，但是代码本身是有些问题的


////改正
//void GetMemory(void** p, int num)
//{
//    *p = (char*)malloc(num);
//}
//void Test(void)
//{
//    char* str = NULL;
//    GetMemory(&str, 100);
//    strcpy(str, "hello world");
//    printf(str);
//
//    //
//    free(str);
//    str = NULL;
//}
//int main()
//{
//    Test();
//    return 0;
//}




//4、

//void Test(void)
//{
//    char* str = (char*)malloc(100);
//    strcpy(str, "hello");
//    free(str);
//    if (str != NULL)
//    {
//        strcpy(str, "world");
//        printf(str);
//    }
//}
//int main()
//{
//    Test();
//    return 0;
//}
//虽然输出的是world，但是这个结果是不对的，原因是代码本身就有问题
//篡改动态内存区的内容，后果难以预料，非常危险，因为free（str）之后,str成为野指针
//if(str!=NULL)语句是不起作用的


////改正：
//void Test(void)
//{
//    char* str = (char*)malloc(100);
//    strcpy(str, "hello");
//    free(str);//free释放str指向的空间后，并不会把str置为NULL
//    str = NULL;
//    if (str != NULL)//如果将这一行去掉也是不行的，这样的话就是将world放入了NULL空指针中
//        //这样是错误的，不能将数据放入空指针中去
//    {
//        strcpy(str, "world");
//        printf(str);
//    }
//}
//int main()
//{
//    Test();
//    return 0;
//}



//C/C++程序的内存开辟

//PDF中有图

//全局变量是放置到静态区中，用关键字static放入也是可以的
//局部变量放入栈区中
//malloc,realloc,calloc这三个动态开辟空间中是将开辟的数据放入到堆中


//C/C++程序内存分配的几个区域：
  //1、栈区（stack）：在执行函数时，函数内部变量的存储单元都可以在栈上创建，函数执行结束时
//这些存储单元自动被释放。栈内存分配运算内置于处理器的指令集中，效率很高，但是分配的内存容
//量有限。栈区主要存放运行函数而分配的局部变量、函数参数、返回数据、返回地址等。
  //2、堆区（heap）：一般由程序员分配释放，若程序员不释放，程序结束时可能有OS(操作系统)回收
//分配方式类似于链表。
  //3、数据段（静态区）（static）存放全局变量、静态数据。程序结束后由系统释放
  //4、代码段：存放函数体（类成员函数和全局函数）的二进制代码。


//有了这幅图，我们就可以很好理解在《C语言初始》中讲的static关键字修饰局部变量的例子

//实际上普通的局部变是在 栈区 分配空间的，栈区的特点是在上面创建的变量出了作用域就销毁。
//但是被static修饰的变量存放在 数据段（静态区），数据段的特点是在上面创建的变量，直到程序
//结束才销毁，所以生命周期变长。




//柔性数组


//也许你从来没有听过柔性数组（flexible array）这个概念，但是它真的存在。C99中，结构中的最后
//一个元素允许是未知大小的数组，这就叫【柔性数组】成员

//struct S
//{
//    int n;
//    int arr[10];//这是10个元素的整型数组
//};
//int main()
//{
//    struct S s;
//    return 0;
//}

//struct S
//{
//    int n;
//    int arr[0];//这是未知大小的 - 柔性数组成员 - 数组的大小是可以调整的
//};
//int main()
//{
//    struct S s;
//    printf("%d\n", sizeof(s));//这里打印出来的是4，因为柔性数组最开始是不占内存空间的
//    //这是因为柔性数组的性质就是需要多少，就开辟多少，这是通过动态内存函数来实现的
//    return 0;
//}


//例子，打印0 1 2 3 4 5 6 7 8 9

//struct S
//{
//    int n;
//    int arr[0];
//};
//int main()
//{
//    struct S* ps = (struct S*)malloc(sizeof(struct S) + 5 * sizeof(int));
//    ps->n = 100;
//    int i = 0;
//    for (i = 0; i < 5; i++)
//    {
//        ps->arr[i] = i;//0 1 2 3 4
//    }
//    struct S* ptr = realloc(ps, 44);
//    if (ptr != NULL)
//    {
//        ps = ptr;
//    }
//    for (i = 5; i < 10; i++)
//    {
//        ps->arr[i] = i;
//    }
//    for (i = 0; i < 10; i++)
//    {
//        printf("%d ", ps->arr[i]);
//    }
//    return 0;
//}


////也可以这样：
//#include <stdlib.h>
//struct S
//{
//    int n;
//    int* arr;
//};
//int main()
//{
//    struct S* ps = (struct S*)malloc(sizeof(struct S));
//    ps->arr = malloc(5 * sizeof(int));//两次malloc是为了让ps->arr是动态的，不然ps被系统释放了
//    //arr指向的动态空间就找不到了
//    int i = 0;
//    for (i = 0; i < 5; i++)
//    {
//        ps->arr[i] = i;//ps->arr[i]//是逐渐找到每个元素，然后用i来代替
//    }
//    for (i = 0; i < 5; i++)
//    {
//        printf("%d ", ps->arr[i]);
//    }
//    //调整一下大小
//    int* ptr = realloc(ps->arr, 10 * sizeof(int));//因为ps->arr指向了结构体中的arr数组
//    //由于arr数组前面的类型是int*，所以在这ptr前就用整型指针
//    if (ptr != NULL)
//    {
//        ps->arr = ptr;
//    }
//    for (i = 5; i < 10; i++)
//    {
//        ps->arr[i] = i;
//    }
//    for (i = 0; i < 10; i++)
//    {
//        printf("%d ", ps->arr[i]);
//    }
//    //释放内存
//    free(ps->arr);
//    ps->arr = NULL;
//    free(ps);
//    ps = NULL;
//    //这里释放两次的原因是这是含有两份空间的，所以先释放在ps中存放的arr的地址
//    //再释放ps整个的地址，动态用来两份空间，所以就需要分别释放两份空间
//    //即两次malloc两次free
//    return 0;
//}



//在上述两种代码中，可以完成相同的功能，当然两种代码各有各的好处：

//第一个代码：  好处是   方便内存释放

//如果我们的代码在一个给别人用的函数中，你在里面做了二次内存分配，并且把整个结构体返回给用户
//用户调用free可以释放结构体，但是用户并不知道这个结构体内的成员也需要free，所以你不能指望用
//户来发现这个事。所以，如果我们把结构体的内存以及其成员要的内存一次性分配好了，并返回给用户
//一个结构体指针，用户做一次free就可以把所有的内存也给释放掉。

//第二个代码： 好处是   有利于访问速度

//连续的内存有益于提高访问速度，也有益于减少内存碎片
//内存碎片就是一段空间没用完之后，剩下的一部分就是浪费的


//柔性数组的特点：
//1、结构中的柔性数组成员前面必须至少一个其他成员
//2、sizeof返回的这种结构大小不包括柔性数组的内存
//3、包含柔性数组成员的结构用malloc()函数进行内存的动态分配，并且分配的内存应该大于结构的大小，
//以适应柔性数组的预期大小。

//柔性数组的优势是安全，运行快，内存占用少



//在电脑内部中，由硬盘，内存，高速缓存，寄存器四部分来分配内存空间
//硬盘，内存，高速缓存，寄存器
//从左到右，内存空间越来越小，但是CPU读取，运算速度是越来越快的

//CPU要满足一个局部性原理
//是指CPU访问存储器时，无论是存取指令还是存取数据，
//所访问的存储单元都趋于聚集在一个较小的连续区域中。



