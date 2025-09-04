/*						题意
		2个多项式,分别给出n1,n2对数据,每对数据有2个值分别表示指数、
	系数,求合并这2个多项式的多项式
		词汇:merge合并,polynomial(poly)多项式,coefficient(coef)系数,
	exponent(expon)指数
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 20

typedef int val_type;

/*	方法Ⅰ数组,ai代表指数为i的项的系数	*/
void solution1( void );
int arr_input( int *a, int *mx_expon );

/*	方法Ⅱ动态/可变数组	*/
typedef struct
{
	val_type *expon;
	val_type *coef;
	int real_size;
	int total_size;
} Poly;
void solution2( void );
int poly_input( Poly *a );
Poly *poly_create( int init_size );
val_type *coef_visit( Poly *a, int index );
val_type *expon_visit( Poly *a, int index );
void poly_inflate( Poly *a, int more_size );
void poly_sort( Poly *a, int len );
void poly_delete_zero( Poly *a, int len );
void arr_free( Poly *a );
//int cmp( const void *a, const void *b );
Poly *poly_sum( Poly *a, int lena, Poly *b, int lenb );

/*	方法Ⅲ链表	*/
typedef struct _node
{
	val_type expon;
	val_type coef;
	struct _node *next;
} Node;

typedef struct _list
{
	Node *head;
	Node *tail;
	int len;
} List;

void solution3( void );
List *list_create( void );
List *list_input( List *list );
void list_add( List *list, val_type expon, val_type coef );
List *list_merge( List *list1, List *list2 );
void list_sort( List *list );
void list_delete_zero( List *list );
void list_clear( List *list );

int main()
{
//	solution1();
//	solution2();
	solution3();
	
	return 0;
}

/*	方法Ⅰ数组,ai代表指数为i的项的系数	*/
void solution1( void )
{
	int poly1[1000] = { 0 };
	int poly2[1000] = { 0 };
	
	int mx_expon = -1;
	arr_input( poly1, &mx_expon );
	arr_input( poly2, &mx_expon );
	
	int poly[1000] = { 0 };
	for ( int i = 0; i <= mx_expon; ++i ){
		poly[i] = poly1[i] + poly2[i];
	}
	
	for ( int i = mx_expon; i >= 0; --i ){
		if ( poly[i] ){
			printf("expon: %d,coef: %d\n", i, poly[i] );
		}
	}
}

int arr_input( int *a, int *mx_expon )
{
	printf("输入多项式的项数:\t");
	int len; scanf("%d", &len );
	
	int coef, expon; 
	for ( int j = 0; j < len; ++j ){
		printf("输入第%d项的指数、系数:\t", j + 1);
		scanf("%d", &expon );
		scanf("%d", &coef );
		if ( expon > *mx_expon ) *mx_expon = expon;
		a[ expon ] = coef;
	}
	
	printf("\n");
	return len;
}

/*	方法Ⅱ可变数组	*/
void solution2( void )
{
	Poly *poly1 = poly_create( BLOCK_SIZE );
	Poly *poly2 = poly_create( BLOCK_SIZE );
	
	int n1 = poly_input( poly1 );
	int n2 = poly_input( poly2 );
	
	poly_sort( poly1, n1 );
	poly_sort( poly2, n2 );
	
	Poly *poly = poly_sum( poly1, n1, poly2, n2 );
	int n = poly -> real_size;
	printf("这2个多项式之和的每一项指数、系数如下\n");
	for ( int i = 0; i < n; ++i ){
		printf("expon: %d,coef: %d\n", poly -> expon[i], poly -> coef[i] );
	}
	
	arr_free( poly );
	arr_free( poly1 );
	arr_free( poly2 );
}

int poly_input( Poly *a )
{
	printf("输入多项式的项数:\t");
	int n; scanf("%d", &n );
	
	printf("依次输入每一项的指数、系数\n");
//	int expon, coef;
	for ( int i = 0; i < n; ++i ){
		printf("第%d项:\t", i + 1 );
//		scanf("%d", &expon ); scanf("%d", &coef );
//		*expon_visit( a, i ) = expon;
//		*coef_visit( a, i ) = coef;
		scanf("%d", expon_visit( a, i ) );
		scanf("%d", coef_visit( a, i ) );
	}
	
	a -> real_size = n;
	return n;
}

Poly *poly_create( int init_size )
{
	Poly *a = ( Poly *) malloc( sizeof( Poly ) );/*	若直接使用未初始化的指针,
		就会造成未定义行为,因为指针可能指向任意内存地址
	Poly *a;只是声明了一个指向Poly结构体的指针a,但并没有对其进行初始化,也就是
	a没有指向任何有效的Poly结构体实例 => [Warning] 'a' is used uninitialized
		所以需要先给a动态分配内存,再调用它的成员
		也可以选择不定义结构体指针直接定义结构体变量Poly a				*/
	a -> expon = ( val_type *) malloc( sizeof( val_type ) * init_size );
	a -> coef = ( val_type *) malloc( sizeof( val_type ) * init_size );
	a -> real_size = 0;
	a -> total_size = init_size;
	return a;
}

val_type *expon_visit( Poly *a, int index )
{
	if ( a -> total_size <= index + 1 ){
		int more_size = ( a -> total_size / BLOCK_SIZE + 1 ) * BLOCK_SIZE - a -> total_size;
		poly_inflate( a, more_size );
	}
	return &( a -> expon[index] );
}

val_type *coef_visit( Poly *a, int index )
{
	return &( a -> coef[index] );
}

void poly_inflate( Poly *a, int more_size )
{
	int new_size = a -> total_size + more_size;
	val_type *expon = ( val_type *) malloc( sizeof( val_type ) * new_size );
	val_type *coef = ( val_type *) malloc( sizeof( val_type ) * new_size );
	
	memcpy( expon, a -> expon, sizeof( val_type ) * a -> real_size );
	memcpy( coef, a -> coef, sizeof( val_type ) * a -> real_size );
	
	free( a -> expon ); free( a -> coef );
	a -> expon = expon; a -> coef = coef;
	
	a -> total_size = new_size;
}

void poly_sort( Poly *a, int len )
{
//	qsort( 数组名, 数组长度, 1个数组元素所占大小, 排序规则 )
//	qsort( a, n, sizeof( val_type ), cmp );
	
	int flag = 0; val_type temp;
	for ( int i = 0; i < len; ++i ){
		for ( int j = 0; j < len - i - 1; ++j ){
			if ( a -> expon[j] < a -> expon[j + 1] ){//按照指数降序排列
				temp = a -> expon[j];
				a -> expon[j] = a -> expon[j + 1];
				a -> expon[j + 1] = temp;
				
				temp = a -> coef[j];
				a -> coef[j] = a -> coef[j + 1];
				a -> coef[j + 1] = temp;
				
				flag = 1;
			}
		}
		if ( flag ) flag = 0;
		else break;
	}
}

//int cmp( const void *a, const void *b )
//{
//	return *( int *) a - *( int *) b;//a - b为升序(递增),反过来降序(递减)
//}

Poly *poly_sum( Poly *a, int lena, Poly *b, int lenb )
{
	Poly *poly = poly_create( lena + lenb );
	int *len = &poly -> real_size;
	
	for ( int i = 0; i < lena; ++i ){
		poly -> expon[i] = a -> expon[i];
		poly -> coef[i] = a -> coef[i];
	}
	for ( int j = lena; j < lena + lenb; ++j ){
		poly -> expon[j] = b -> expon[j - lena];
		poly -> coef[j] = b -> coef[j - lena];
	}	
	*len = lena + lenb;
	
	int i = 0, j = lena;
	for ( ; i < lena && j < lena + lenb; ){//把&&换成逗号好呛不得行
		if ( poly -> expon[i] < poly -> expon[j] ){
			++j;
		} else if ( poly -> expon[i] > poly -> expon[j] ){
			++i;
		} else {
			int coef = poly -> coef[i] + poly -> coef[j];
			//由于2项合并后只剩1项,于是清除1项(j)(让系数、指数为0),用另1项(i)装新系数
			poly -> coef[j] = 0;
			poly -> coef[i] = coef;
			++i; ++j;
		}
	}
	//系数为0的项统一在delete_zero函数中修改指数为0 => 清除
	poly_delete_zero( poly, lena + lenb );
	poly_sort( poly, a -> real_size );
	return poly;
}

void poly_delete_zero( Poly *a, int len )
{
	for ( int i = 0; i < len; ++i ){
		if ( a ->coef[i] == 0 ){
			a -> expon[i] = 0;
			a -> real_size --;
		}
	}
/*		在遍历一次后让系数为0的项的指数也为0,起到"删除"作用,
	但实际上该项还保留在数组中,并且由于poly_sort函数是根据指
	数来降序排列,不是根据系数来排序,于是直接用poly_sort会出
	现指数为0系数非0的项排到外面的情况
		所以需要把这一个特殊项移到a -> real_size范围内
*/
	int mark = 0;
	for ( int i = 0; i < len; ++i ){
		if ( a -> coef[i] == 0 ){
			mark = i;
			break;
		}
	}
	
	for ( int i = mark + 1; i < len; ++i ){
		if ( a -> coef[i] != 0 && a -> expon[i] == 0 ){
			a -> coef[mark] = a -> coef[i];
			a -> expon[mark] = 0;
			a -> coef[i] = 0;
			break;
		}
	}
}

void arr_free( Poly *a )
{
	free( a -> expon ); free( a -> coef );
	a -> real_size = 0;
	a -> total_size = 0;
}

/*	方法Ⅲ链表	*/
void solution3( void )
{
	List *poly1 = list_create(), *poly2 = list_create();
	list_input( poly1 ); list_input( poly2 );
	
	List *poly = list_merge( poly1, poly2 );
	
	Node *p = poly -> head;
	for ( int i = 0 ; p; ++i, p = p -> next ){
		printf("第%d项,指数: %d,系数: %d\n", i + 1,p -> expon, p -> coef );
	}
	
	list_clear( poly );
	list_clear( poly1 );
	list_clear( poly2 );
}

List *list_create( void )
{
	List *list = ( List *) malloc( sizeof( List ) );
	list -> head = list -> tail = NULL;
	list -> len = 0;
	return list;
}

List *list_input( List *list )
{
	printf("输入多项式的项数:\t");
	int len; scanf("%d", &len );
//	list -> len = len;由于在list_add()函数已经由len ++了,此处无需再单独赋值
	
	val_type coef, expon; 
	for ( int j = 0; j < len; ++j ){
		printf("输入第%d项的指数、系数:\t", j + 1);
		scanf("%d", &expon );
		scanf("%d", &coef );
		list_add( list, expon, coef );
	}
	
	return list;
}

void list_add( List *list, val_type expon, val_type coef )
{
	Node *node = ( Node *) malloc( sizeof( Node ) );
	node -> expon = expon;
	node -> coef = coef;
	node -> next = NULL;
	
	if ( list -> tail ){
		while ( list -> tail -> next ){
			list -> tail = list -> tail -> next;
		}
		list -> tail = list -> tail -> next = node;
	} else {
		list -> head = list -> tail = node;
	}
	
	list -> len ++;
}

void list_sort( List *list )
{
	int flag = 0;//仿照冒泡排序
/*
//	思路1
	//想象最左边1个q指向一个结点,r保持指向q右边的一个结点,然后q,r一起不断右移一格
	for ( Node *p = list -> head; p; p = p -> next ){
		for ( Node *q = p, *r = q -> next; r ; ){
			if ( q -> expon < r -> expon ){//按指数降序排序
				//注意:链表的交换实际上是改变结点的指针域 => 改变结点指向的下一个结点
				q -> next = r -> next;
				r -> next = q;//此时q在r右边(链表发生交换)
				flag = 1;
				r = q -> next;
			} else {//q在r左边
				q = r;
				r = r -> next;
			}
		}
		if ( flag ) flag = 0;
		else break;
	}
	
//	思路1变式,然而并不可以
	for ( Node *p = list -> head; p -> next; p = p -> next ){
		for ( Node *q = list -> head; q; q = q -> next ){
			if ( p -> expon < q -> expon ){
				p -> next = q -> next;
				q -> next = p;
				
				flag = 1;
			}
		}
		if ( flag ) flag = 1;
		else break;
	}

//	思路2,尝试只交换数据域,不交换结点
	for ( Node *p = list -> head; p -> next; p = p -> next ){
		for ( Node *q = list -> head; q; q = q -> next ){
			if ( p -> expon < q -> expon ){
				flag = 1;
				val_type temp = p -> expon;
				p -> expon = q -> expon;
				q -> expon = temp;
				//细节:2个结点的所有数据都要交换
				temp = p -> coef;
				p -> coef = q ->coef;
				q -> coef = temp;
			}
		}
//		if ( flag ) flag = 0;
//		else break;
	}
*/	
	//思路2改进,这个可以,思路1、2不得行
	for ( Node *p = list -> head; p; p = p -> next ){
		for ( Node *q = list -> head, *r = q -> next; r; q = r, r = r -> next ){
			if ( q -> expon < r ->expon ){
				flag = 1;
				
				val_type temp = q -> expon;
				q -> expon = r -> expon;
				r -> expon = temp;
				
				temp = q -> coef;
				q -> coef = r ->coef;
				r -> coef = temp;
			}
		}
		if ( flag ) flag = 0;
		else break;
	}
}

List *list_merge( List *list1, List *list2 )
//照抖poly_sum()改就行,甚至改出来还更安逸
{
	List *list = list_create();
	list_sort( list1 ); list_sort( list2 );
	
/*		想象2排结点(按指数降序排列),p1指向上面的,p2指向下面的。
	不断比较p1、p2指向结点的指数,让较大的那个右移来变小,同时记
	录较大的数据,直到p1、p2指向的指数相等,进行运算,存进新链表,
	p1、p2同时右移
		移动过程中,可能出现某个链表已经遍历完的情况 => 此时p为
	空,那么不能继续比较,则把还没有遍历完的那个链表剩下的部分存
	进新链表,避免漏算
*/	
	Node *p1 = list1 -> head, *p2 = list2 -> head;
	for ( ; p1 || p2; ){//结束条件:p1、p2都为空 => 2个链表都遍历完
		if ( p1 && p2 ){
			if ( p1 -> expon > p2 -> expon ){
				list_add( list, p1 -> expon, p1 -> coef );
				p1 = p1 -> next;
			} else if ( p1 -> expon < p2 -> expon ){
				list_add( list, p2 -> expon, p2 -> coef );
				p2 = p2 -> next;
			} else {
				list_add( list, p1 -> expon, p1 -> coef + p2 -> coef );
				p1 = p1 -> next; p2 = p2 -> next;
			}		
		} else if ( p1 ){
			list_add( list, p1 -> expon, p1 -> coef );
			p1 = p1 -> next;
		} else if ( p2 ){
			list_add( list, p2 -> expon, p2 -> coef );
			p2 = p2 -> next;
		}
	}
	
	list_delete_zero( list );
	
	return list;
}

void list_delete_zero( List *list )
{
/*									思路1
	错因:删除了结点但是没有考虑让上一个结点链接下一个结点,导致链表中断
	for ( Node *p = list -> head, *q = NULL; p; p = q, q = q -> next ){
		if ( p -> coef == 0 ){
			if ( p == list -> head ){
				free( p );
				list -> head = q;
			} else {
				free( p );
			}
			list -> len --;
		}
		if ( q == NULL ) break;
			q为空 => p赋值为q后p也空,此时已经到链表尽头,并且由于空指针
		不能进行q -> next访问,所以直接break
			也可以理解成pq相邻,p左q右,检查完p后若q为空,那不消继续检查了
		=> 已经检查完了,直接break									
	}																	*/
//					思路2
	for ( Node *p = list -> head, *q = NULL; p; ){
		if ( p -> coef == 0 ){
			if ( p == list -> head ){
				p = p -> next;
				free( list -> head );
				list -> head = p;
			} else {
				q -> next = p -> next;
				free( p );
				p = q -> next;//p一定要一格一格往右挪,同时保持q在p左边相邻
			}
			list -> len --;
		} else {
			//一直遍历,清除所有系数为0的结点
			q = p;
			p = p -> next;
		}
	}
	
/*				思路3,与思路2类似
	for ( Node *p = list -> head, *q = NULL; p; ){
		if ( p -> coef == 0 ){
			if ( q ){
				//由于要访问q -> next,需考虑q的存在性
				q -> next = p -> next;
				free( p );
				p = q -> next;
			} else {
				//q不存在时即p为表头时
				p = p -> next;
				free( list -> head );
				list -> head = p;
			}
			list -> len --;
		} else {
			q = p;
			p = p -> next;
		}
	}											*/
}

void list_clear( List *list )
{
	for ( Node *p = list -> head; p; ){
		Node *temp = p -> next;
		free( p );
		p = temp;
	}
	list -> head = list -> tail = NULL;
	list -> len = 0;
}
