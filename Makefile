#目标文件:依赖文件
#[TAB]指令

#第一个目标文件是最终目标
OBJ=DuLinkList.o Qsort.o pstree.o
FT=test
${FT}:${OBJ}
	gcc ${OBJ} -o test
DuLinkList.o:DuLinkList.c DuLinkList.h
	gcc -c DuLinkList.c -o DuLinkList.o
Qsort.o:Qsort.c Qsort.h
	gcc -c Qsort.c -o Qsort.o
pstree.o:pstree.c main.h
	gcc -c pstree.c -o pstree.o
	
#伪目标 .PHONY(伪造的,假货,赝品)
.PHONY:
cl:
	rm -rf ${OBJ}
	rm -rf ${FT}
	
run:
	make
	make cl
	./${FT}