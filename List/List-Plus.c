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
void list_remove( List *list, val_type target );//链表的删除(remove)
void list_clear( List *list );//链表的清除(clear all)
/*	额外内容	*/
List list_create( void );//创建链表
void list_insert_head( List *list, val_type num );//插入头结点(add head)
void list_popback( List *list );//删除尾结点
void list_popfront( List *list );//删除头结点
/*	浙大DS补充	*/
int list_length( List *list );//求表长
Node *list_findkth( List *list, int pos );//按序号查找第k(pos)个结点
Node *list_findx( List *list, val_type x );//按值查找
Node *list_insert( List *list, int pos, val_type val );//在第i(pos)个结点后插入1个结点
Node *list_delete( List *list, int pos );//删除第i(pos)个结点

int main( int argc, const char *argv[] )
{
	List *list;
	list -> head = list -> tail = NULL;
	
	val_type num;
	while ( scanf("%d", &num ) != EOF ){
		list_add( list, num );
	}
	
	list_print( list );
/*
	Node *head = NULL;
	val_type num;
	
	while ( scanf("%d", &num ) != EOF ){
		Node *temp = ( Node *) malpos( sizeof( Node ) );
		temp -> value = num;
		temp -> next = NULL;
		
		Node *last = head;
		if ( last ){
			while ( last -> next ){
				last = last -> next;
			}
			last -> next = temp;
		} else {
			head = temp;
		}
	}
*/
	
	return 0;
}

				/*	课堂内容	*/

//链表的添加
void list_add( List *list, val_type num )
{
	Node *temp = ( Node *) malloc( sizeof( Node ) );
	temp -> value = num;
	temp -> next = NULL;
/*
	if ( list -> tail ){
		while ( list -> tail -> next ){
			list -> tail = list -> tail -> next;
		}
		list -> tail = list -> tail -> next = temp;
	} else {
		list -> head = list -> tail = temp;
	}
*/
	if ( list -> tail ){
		list -> tail = list -> tail -> next = temp;
	} else {
		list -> head = list -> tail = temp;
	}
}

//链表的遍历
void list_print( List *list )
{
	for ( Node *p = list -> head; p; p = p -> next ) printf("%d\t", p -> value );
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
void list_remove( List *list, val_type target )
{
	int isfound = 0;
	for ( Node *p = list -> head, *q = NULL; p; q = p, p = p -> next ){
		if ( p -> value == target ){
			if ( q ){
				q -> next = p -> next;
			} else {
				list -> head = p -> next;//head = head -> next;
			}
			isfound = 1;
			free( p ); p = NULL;
			break;
		}
	}
	
	if ( !isfound ) printf("not found!\n");
}

//链表的清除
void list_clear( List *list )
{
	for ( Node *p = list -> head, *q = p; p; q = p ){
		p = p -> next;
		free ( q );
	}
/*
	for ( Node *p = list -> head -> next; p; ){
		free( list -> head );
		list -> head = p;
	}
	free( list -> head );
	free( list -> tail );
	list -> head = list -> tail = NULL;
*/
}

				/*	额外内容	*/
//创建链表
List list_create( void )
{
	List list;
	list.head = list.tail = NULL;
	return list;
}

//插入头结点(add head)
void list_insert_head( List *list, val_type num )
{
	Node *temp = ( Node *) malloc( sizeof( Node ) );
	temp -> value = num;
	temp -> next = list -> head;
	list -> head = temp;
}

//删除尾结点
void list_popback( List *list )
{
	Node *scd_to_last = list -> head;
	while ( scd_to_last -> next != list -> tail ){
		//如果scd_to_last为空 => head = NULL => tail必空
		scd_to_last = scd_to_last -> next;
	}
	
	free( list -> tail );
	list -> tail = scd_to_last;
/*	
	Node *new_tail = list -> head;
	free( list -> tail );
	list -> tail = NULL;
	
	if ( new_tail ){
		while ( new_tail -> next ){
			new_tail = new_tail -> next;
		}
		list -> tail = new_tail;
	}
*/
/*
	for ( Node *p = list -> head; ; p = p -> next ){
		if ( p -> next == list -> tail ){
			free( list -> tail );
			list -> tail = p;
			break;
		}
	}
*/
}

//删除头结点
void list_popfront( List *list )
{
	Node *new_head = list -> head -> next;
	free( list -> head );
	list -> head = new_head;
}


/*	浙大DS补充	*/
int list_length( List *list )//求表长
{
	int len = 1;
	for ( Node *p = list -> head; p; p = p -> next ) len ++;
	return len;
}

Node *list_findkth( List *list, int pos )//按序号查找第k(pos)个结点
{
	Node *ret = NULL;
	int len = list_length( list );
	if ( 1 <= pos && pos <= len ){
		//len为结点数,于是1 ≤pos ≤len可保证序号合法,并且查找的结点不为空
		ret = list -> head;//第1个结点
		for ( int i = 2; i <= pos; ++i ) ret = ret -> next;//i从2开始
	}
	return ret;
	//返回NULL或者目标结点,于是在main中调用该函数时需要先判断返回值是否为空
/*	慕课写法
	Node *p = list -> head;
	int i = 1;
	while ( p != NULL && i < k ){//k = pos
		p = p -> next;
		i++;s
	}
	if ( i == k ) return p;//找到第k个结点,返回指针
	else return NULL;//没找到,返回空
*/
}

Node *list_findx( List *list, val_type x )//按值查找
{
	Node *ret = list -> head;
	while ( ret != NULL && ret -> value != x ){
		ret = ret -> next;
	}
	return ret;//只需在函数调用者处判断返回值是否空即可
}

Node *list_insert( List *list, int pos, val_type val )//在第i(pos)个结点后插入1个结点
{
	//保证序号合法
	int len = list_length( list );
	if ( pos < 1 || pos > len ) return NULL;
	//得到第pos个结点
	Node *p = list -> head;
	for ( int i = 2; i <= pos; ++i ) p = p -> next;//i从2开始
	//插入
	Node *temp = p -> next;
	Node *insert = ( Node *) malloc( sizeof( Node ) );
	insert -> value = val;
	insert -> next = temp;
	p -> next = insert;
	
	return p;
}

Node *list_delete( List *list, int pos )//删除第i(pos)个结点
{
	//保证序号合法
	int len = list_length( list );
	if ( pos < 1 || pos > len ) return NULL;
	//得到第(pos - 1)个结点
	Node *p = list -> head;
	for ( int i = 2; i < pos; ++i ) p = p -> next;//i从2开始
	//删除
	Node *temp = p -> next -> next;//第(pos + 1)个结点
	free( p -> next );//第pos个结点
	p -> next = temp;
	
	return p;
}
