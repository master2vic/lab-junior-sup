# Lab 1

## Exercise 1

### :grey_question:操作系统镜像文件ucore.img是如何一步一步生成的？

### :arrow_forward:Answer

使用`vim makefile`打开`makefile`文件。找到关键字`UCOREIMG`。发现如下字段

~~~makefile
# create ucore.img
UCOREIMG	:= $(call totarget,ucore.img)

$(UCOREIMG): $(kernel) $(bootblock)
	$(V)dd if=/dev/zero of=$@ count=10000
	$(V)dd if=$(bootblock) of=$@ conv=notrunc
	$(V)dd if=$(kernel) of=$@ seek=1 conv=notrunc

$(call create_target,ucore.img)
~~~

#### `UCOREIMG	   :=$(call totarget, ucore.img)`

需要知道`totatget`是什么，但是在这个makefile中就没找到，后来发现其实这个makefile包含另一个mk。（在第87行。）	

~~~makefile
include tools/function.mk
~~~

而在另一个文件中`totarget`是这样定义的：

~~~makefile
totarget = $(addprefix $(BINDIR)$(SLASH),$(1))
~~~

所以其实`UCOREIMG	:= $(call totarget,ucore.img)`就是在`ucore.img`前面加上`$(BINDIR)$(SLASH)`

其中`$(BINDIR)`,`$(SLASH)`分别是：`bin`,`/`。所以`UCOREIMG`就是`bin/uncore.img`。

#### `$(UCOREIMG): $(kernel) $(bootblock)`

`$(UCOREIMG)`依赖`$(kernel)`和`$(bootblock)`。而`kernel`,`bootblock`。

#### `$(V)dd if=/dev/zero of=$@ count=10000`

> 备注：在类UNIX 操作系统中, /dev/zero 是一个特殊的文件，当你读它的时候，它会提供无限的空字符(NULL, ASCII NUL, 0x00)。其中的一个典型用法是用它提供的字符流来覆盖信息，另一个常见用法是产生一个特定大小的空白文件。BSD就是通过mmap把/dev/zero映射到虚地址空间实现共享内存的。可以使用mmap将/dev/zero映射到一个虚拟的内存空间，这个操作的效果等同于使用一段匿名的内存（没有和任何文件相关）。

#### `$(V)dd if=$(bootblock) of=$@ conv=notrunc`

将bin/bootblock复制到bin/ucore.img

#### `$(V)dd if=$(kernel) of=$@ seek=1 conv=notrunc`

继续将bin/kernel复制到bin/ucore.img，这里使用了选项`seek=1`，意思是：复制时跳过bin/ucore.img的第一个block，从第2个block也就是第512个字节后面开始拷贝bin/kernel的内容。原因是显然的：ucore.img的第1个block已经用来保存bootblock的内容了。

#### `$(call create_target,ucore.img)`

由于只有一个输入参数，因此这里会直接返回。

### :grey_question:一个被系统认为是符合规范的硬盘主引导扇区的特征是什么？

~~~makefile
# create bootblock
bootfiles = $(call listf_cc,boot)
$(foreach f,$(bootfiles),$(call cc_compile,$(f),$(CC),$(CFLAGS) -Os -nostdinc))

bootblock = $(call totarget,bootblock)

$(bootblock): $(call toobj,$(bootfiles)) | $(call totarget,sign)
	@echo + ld $@
	$(V)$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 $^ -o $(call toobj,bootblock)
	@$(OBJDUMP) -S $(call objfile,bootblock) > $(call asmfile,bootblock)
	@$(OBJCOPY) -S -O binary $(call objfile,bootblock) $(call outfile,bootblock)
	@$(call totarget,sign) $(call outfile,bootblock) $(bootblock)

$(call create_target,bootblock)
~~~

`$(bootblock): $(call toobj,$(bootfiles)) | $(call totarget,sign)`这行可以看出`bootblock`依赖于`sign`。

~~~c
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

int
main(int argc, char *argv[]) {
    struct stat st;
    if (argc != 3) {
        fprintf(stderr, "Usage: <input filename> <output filename>\n");
        return -1;
    }
    if (stat(argv[1], &st) != 0) {
        fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
        return -1;
    }
    printf("'%s' size: %lld bytes\n", argv[1], (long long)st.st_size);
    if (st.st_size > 510) {
        fprintf(stderr, "%lld >> 510!!\n", (long long)st.st_size);
        return -1;
    }
    char buf[512];
    memset(buf, 0, sizeof(buf));
    FILE *ifp = fopen(argv[1], "rb");
    int size = fread(buf, 1, st.st_size, ifp);
    if (size != st.st_size) {
        fprintf(stderr, "read '%s' error, size is %d.\n", argv[1], size);
        return -1;
    }
    fclose(ifp);
    buf[510] = 0x55;
    buf[511] = 0xAA;
    FILE *ofp = fopen(argv[2], "wb+");
    size = fwrite(buf, 1, 512, ofp);
    if (size != 512) {
        fprintf(stderr, "write '%s' error, size is %d.\n", argv[2], size);
        return -1;
    }
    fclose(ofp);
    printf("build 512 bytes boot sector: '%s' success!\n", argv[2]);
    return 0;
}
~~~

根据`sign.c`文件可知，其实`buf`的大小就只有512，而最后两个字节`buf[510]`和`buf[512]`分别在代码中设置为`0x55`,`0xAA`。

所以大小为：512字节

最后两字节：`0x55`,`0xAA`。

## Exercise 2

### 从CPU加电后执行的第一条指令开始，单步跟踪BIOS的执行。

根据附录，修改lab1/tools/gdbinit,

```
set architecture i8086
target remote :1234
```

### 在初始化位置0x7c00设置实地址断点,测试断点正常。



### 从0x7c00开始跟踪代码运行,将单步跟踪反汇编得到的代码与bootasm.S和 bootblock.asm进行比较。



### 自己找一个bootloader或内核中的代码位置，设置断点并进行测试。

