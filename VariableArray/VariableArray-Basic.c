//注:4.1可变数组包括以下3集,代码都在这里
//4.1.1可变数组 + 4.1.2可变数组的数据访问 + 4.1.3可变数组

/*			用到的缩写
	array -> arr数组
	length -> len长度(虽然习惯上把size作为"长度")
	initial -> init初始的
	temporary -> temp临时的
	index -> idx指数,索引(下标)
	value -> val值
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct
{
	int *arr;
	int len;
} array;

array array_create( int init_len );
void array_free( array *a );
int array_len( const array *a );
int *array_at( array *a, int idx );
void array_inflate( array *a, int more_len );

int main( int argc, const char *argv[] )
{	
 	array a = array_create( 100 );
 	
 	int number;
 	int cnt = 0;
	while ( 1 ){
		scanf("%d", &number );
		if ( number == -1 ) break;
		*array_at( &a, cnt++ ) = number;
//		scanf("%d", array_at( &a, cnt++ ) );
	}
	
	return 0;
}

array array_create( int init_len )
{
	array temp;
	temp.len = init_len;
	temp.arr = ( int * ) malloc( temp.len * sizeof( int ) );
	return temp;
}

void array_free( array *temp )
{
	free( temp -> arr );//(*p).a == p -> a
	temp -> arr = NULL;
	temp -> len = 0;
}

int array_len( const array *temp )
{
	return temp -> len;
}

const int block_len = 20;
int *array_at( array *temp, int idx )
{
	if ( idx >= temp -> len ){
		array_inflate( temp, block_len );
		//每次增长1个block长度(5,6,...),而不是1个1个地长
//老师写成( temp, ( idx / block_len + 1 ) * block_len - temp -> len )
	}
	return &( temp -> arr[idx] );
}
/*
	*array_at()函数的功能 = 下面2个函数之和
	=> 把这两个函数封装成上面一个函数
int array_visit( array *temp, int idx )
{
	return temp -> arr[idx];
}

void array_modify( const array *temp, int idx, int val )
{
	temp -> arr[idx] = val;
}
*/

//数组"增长"函数
void array_inflate( array *temp, int more_len )
{
	int *new = ( int * ) malloc( ( temp -> len + more_len ) * sizeof( int ) );
	for ( int i = 0; i < temp -> len; ++i ){
		new[i] = temp -> arr[i];
	}
	//也可以用string.h的memcpy函数
	//原型void *memcpy(void *str1, const void *str2, size_t n)
	//=> memcpy( new, temp -> arr, temp -> len );//把arr[0]~arr[len - 1]复制给new
	free( temp -> arr );//或者调用array_free( *temp );
	temp -> arr = new;
	temp -> len += more_len;
}
