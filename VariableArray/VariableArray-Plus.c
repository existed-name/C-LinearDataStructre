#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int arr_type;

typedef struct
{
	arr_type *arr;
	int size;
	int efctive_size;
} Array;

Array arr_create( int init_size );
int arr_size( Array *a );
arr_type *arr_visit( Array *a, int index );
void arr_inflate( Array *a, int more_size );
//仿照vector↓
void arr_pushback( Array *a, arr_type value );
void arr_pushfront( Array *a, arr_type value );
void arr_popback( Array *a );
void arr_popfront( Array *a );
int arr_ifempty( Array *a );
void arr_insert( Array *a, int index, arr_type value );
int arr_find( Array *a, arr_type target );
//仿照vector↑
void arr_free( Array *a );

#define BLOCK_SIZE 20

int main( int argc, char *argv[] )
{
	
	
	return 0;
}

Array arr_create( int init_size )
{
	Array a;
	a.arr = ( arr_type *) malloc( init_size * sizeof( arr_type ) );
	a.size = a.efctive_size = init_size;//假设这init_size个数组单位全是有效值
	return a;
}

int arr_size( Array *a )
{
	return a -> efctive_size;
}

arr_type *arr_visit( Array *a, int index )
{
	if ( a -> size <= index ){
		int more_size = ( a -> size / BLOCK_SIZE + 1 ) * BLOCK_SIZE - a -> size;
		arr_inflate( a, more_size );
	}
	if ( a -> efctive_size <= index ) a -> efctive_size = index + 1;
	return &( a -> arr[index] );
}

void arr_inflate( Array *a, int more_size )
{
	int new_size = a -> size + more_size;
	arr_type *p = ( arr_type *) malloc( new_size * sizeof( arr_type ) );
	
	memcpy( p, a -> arr, new_size * sizeof( arr_type ) );
	free( a -> arr );
	
	a -> arr = p;
	a -> efctive_size = a -> size + 1;
	a -> size = new_size;
}

void arr_free( Array *a )
{
	free( a -> arr );
	a -> arr = NULL;
	a -> size = 0;
	a -> efctive_size = 0;
}

//arr_type *arr_visit( Array *a, int index );↓
arr_type arr_get( Array *a, int index )
{
	return a -> arr[index];
}

void arr_set( Array *a, int index, int value )
{
	a -> arr[index] = value;
}
//arr_type *arr_visit( Array *a, int index );↑

//仿照vector↓
void arr_pushback( Array *a, arr_type value )
{
	
}

void arr_pushfront( Array *a, arr_type value )
{
			
}

void arr_popback( Array *a )
{
	arr_type *p = ( arr_type *) malloc( a -> size * sizeof( arr_type ) );
	memcpy( p, a -> arr, a -> efctive_size * sizeof( arr_type ) );
	
	a -> efctive_size --;
}

void arr_popfront( Array *a )
{
	arr_type *p = ( arr_type *) malloc( a -> size * sizeof( arr_type ) );
	for ( int i = 0; i < a -> efctive_size - 1; ++i ){
		p[i] = a -> arr[i + 1];
	}
	
	a -> efctive_size --;	
}

int arr_ifempty( Array *a )
{
	int ret = 0;
	if ( a -> arr == NULL ) ret = !ret;
	return ret;
}

void arr_insert( Array *a, int index, arr_type value )
{
	if ( a -> size <= index ){
		int more_size = ( a -> size / BLOCK_SIZE + 1 ) * BLOCK_SIZE - a -> size;
		arr_inflate( a, more_size );
	}
	a -> arr[index + 1] = value;
}

int arr_find( Array *a, arr_type target )
{
	int ret = -1;
	for ( int i = 0; i < a -> efctive_size; ++i ){
		if ( a -> arr[i] == target ){
			ret = i;
			break;
		}
	}
	return ret;
}
//仿照vector↑
