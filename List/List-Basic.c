/*	注:本文档只涉及单向链表	*/
/*	注:node有"结点""节点"两种翻译,慕课上用的"结点",此处不纠结	*/
// 4.2 链表
#include <stdio.h>
#include <stdlib.h>

typedef int val_type;

typedef struct _node
{
	val_type value;
	struct _node *next;	
} Node;

typedef struct _list
{
	Node *head;
	Node *tail;
} List;

/*	课堂内容	*/
void list_add( List *list, val_type num );//链表的添加(append tail)
void list_print( List *list );//链表的遍历(traversal)
int list_find( List *list, val_type target );//链表的查找(search)
void list_delete( List *list, val_type target );//链表的删除(remove)
void list_clear( List *list );//链表的清除(clear all)
/*	额外内容	*/
void list_insert_head( List *list, val_type num );//插入头结点(add head)


int main( int argc, const char *argv[] )
{
	List list;
	list.head = list.tail = NULL;
	val_type num;
	
	while ( scanf("%d", &num ) !=EOF ){
		list_add( &list, num );
	}
	
	list_print( &list );
	
	return 0;
}

//链表的添加
void list_add( List *list, val_type num )
{
	Node *temp = ( Node *) malloc( sizeof( Node ) );
	temp -> value = num;
	temp -> next = NULL;
	
	if ( list ){
		while ( list -> tail -> next ){
			list -> tail = list -> tail -> next;
		}
		list -> tail -> next = temp;
	} else {
		list -> head = list -> tail = temp;
	}
}

//链表的遍历
void list_print( List *list )
{
	//从头开始,一直到不存在为止
	for ( Node *temp = list -> head; temp; temp = temp -> next ){
		printf("%d\t", temp -> value );
	}
	printf("\n");
}

//链表的查找
int list_find( List *list, val_type target )
{
	int isfound = 0;
	for ( Node *p = list -> head; p; p = p -> next ){
		if ( p -> value == target ){
			isfound = 1;
			break;
		}
	}
	
	return isfound;
}

//链表的删除
//本质上是让被删除的节点的上一个节点指向被删除节点的下一个节点
void list_delete( List *list, val_type target )
{
	Node *p, *q;//q在p的前面
	for ( q = NULL, p = list -> head; p; q = p, p = p -> next ){
		//每轮循环结束,先q = p后p = p -> next以记录相邻2个节点
		if ( p -> value == target ){
			/*		边界情况判定
				p.next或者p -> next来引用结构体成员,那么首先要保证p是存在的,即p != NULL
				而我们的代码,for循环的条件就可以判断p是否存在,于是p是安全的
				再看q,发现,没有代码来保证q != NULL,
					于是我们要单独设置一段代码进行q = NULL这种特殊情况的处理
			*/
			if ( q ){
				q -> next = p -> next;
			} else {
				//在q = NULL的时候,也就是还在第一个节点,就找到了目标
				//而q不存在,不能使用q -> next,那么只能使用head -> next
				list -> head = p -> next;
			}
			free( p );
			p = NULL;
			break;
		}
	}
}

//链表的清除
void list_clear( List *list )
{
	//让q = p -> next,而free( p ),从而一直free直到p为空
	Node *p, *q;
	for ( p = list -> head; p; p = q ){
		q = p -> next;
		free( p );
	}
/*	Node *p = list -> head, *q;
	
	while ( p ){
		q = p -> next;
		free( p );
		p = q;
	}
*/
}

//插入头结点(add head)
void list_insert_head( List *list, val_type num )
{
	Node *temp = ( Node *) malloc( sizeof( Node ) );
	temp -> value = num;
	temp -> next = list -> head;
	list -> head = temp;
}
