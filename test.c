// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "array.h"
#include "list_multitype.h"
#include "list.h"
#include "map.h"
#include "queue.h"
#include "stack.h"

#define STACK_SIZE  4
#define MAP_ENTRIES 100000
#define ARRAY_SIZE 100000

void test_queue() {
    printf("%-24s", "Check queue: ");

    // Push int object
    int qi = 0x11111111;
    void *queue = NULL;
    assert(mt_queue_push(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);

    // Push int object
    qi = 0x22222222;
    assert(mt_queue_push(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);

    // Push int object
    qi = 0x33333333;
    assert(mt_queue_push(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);

    // Push double object
    double pi = 3.14159265;
    assert(mt_queue_push(&queue, &pi, sizeof(pi)) == QUEUE_SUCCESS);

    // Push int object
    qi = 0x44444444;
    assert(mt_queue_push(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);

    // Check first object
    assert(mt_queue_front(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x11111111);

    // Check last object
    assert(mt_queue_back(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x44444444);

    // Check length
    assert(mt_queue_length(&queue) == 5);

    // Pop int object
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x11111111);

    // Check length
    assert(mt_queue_length(&queue) == 4);

    // Push int object
    qi = 0x55555555;
    assert(mt_queue_push(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);

    // Check length
    assert(mt_queue_length(&queue) == 5);

    // Pop int object
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x22222222);

    // Check length
    assert(mt_queue_length(&queue) == 4);

    // Pop int object
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x33333333);

    // Pop double object
    memset(&pi, 0, sizeof(pi));
    assert(mt_queue_pop(&queue, &pi, sizeof(pi)) == QUEUE_SUCCESS);
    assert(pi == 3.14159265);

    // Pop int object
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x44444444);

    // Check length
    assert(mt_queue_length(&queue) == 1);

    // Pop int object
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x55555555);

    // Check length
    assert(mt_queue_length(&queue) == 0);

    // Pop from empty stack
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_IS_EMPTY);

    // Push int object
    qi = 0x66666666;
    assert(mt_queue_push(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);

    // Check length
    assert(mt_queue_length(&queue) == 1);

    // Push int object
    qi = 0x77777777;
    assert(mt_queue_push(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);

    // Pop int object
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x66666666);

    // Pop incorrect object
    memset(&pi, 0, sizeof(pi));
    assert(mt_queue_pop(&queue, &pi, sizeof(pi)) == QUEUE_SIZE_ERROR);

    // Pop int object
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_SUCCESS);
    assert(qi == 0x77777777);

    // Pop from empty stack
    memset(&qi, 0, sizeof(qi));
    assert(mt_queue_pop(&queue, &qi, sizeof(qi)) == QUEUE_IS_EMPTY);

    assert(queue == NULL);

    printf("PASS\n");
}

void test_list() {
    printf("%-24s", "Check list: ");

    struct LIST *list = NULL;
    struct LIST object;

    // Add object
    object.field = 0;
    assert(list_add(&list, &object) == LIST_SUCCESS);

    // Add object
    object.field = 1;
    assert(list_add(&list, &object) == LIST_SUCCESS);

    // Add object
    object.field = 2;
    assert(list_add(&list, &object) == LIST_SUCCESS);

    // Add object
    object.field = 3;
    assert(list_add(&list, &object) == LIST_SUCCESS);

    // Get head
    assert(list_get_node(&list, &object, 0) == LIST_SUCCESS);
    assert(object.field == 0);

    // Get tail
    assert(list_get_node(&list, &object, 3) == LIST_SUCCESS);
    assert(object.field == 3);

    // Get tail with negative index
    assert(list_get_node(&list, &object, -1) == LIST_SUCCESS);
    assert(object.field == 3);

    // Get before tail
    assert(list_get_node(&list, &object, -2) == LIST_SUCCESS);
    assert(object.field == 2);

    // Get out of range positive
    assert(list_get_node(&list, &object, 4) == LIST_OUT_OF_RANGE);

    // Get out of range negative
    assert(list_get_node(&list, &object, -5) == LIST_OUT_OF_RANGE);

    // Insert before head
    object.field = -1;
    assert(list_insert(&list, &object, 0) == LIST_SUCCESS);
    assert(list_get_node(&list, &object, 0) == LIST_SUCCESS);
    assert(object.field == -1);

    // Insert into center
    object.field = 11;
    assert(list_insert(&list, &object, 2) == LIST_SUCCESS);
    assert(list_get_node(&list, &object, 2) == LIST_SUCCESS);
    assert(object.field == 11);

    // Insert before tail
    object.field = 22;
    assert(list_insert(&list, &object, -1) == LIST_SUCCESS);
    assert(list_get_node(&list, &object, -2) == LIST_SUCCESS);
    assert(object.field == 22);

    // Delete head
    assert(list_del(&list, 0) == LIST_SUCCESS);

    // Delete tail
    assert(list_del(&list, -1) == LIST_SUCCESS);

    // Delete from center
    assert(list_del(&list, 2) == LIST_SUCCESS);

    // Delete after head
    assert(list_del(&list, 1) == LIST_SUCCESS);

    // Delete before tail
    assert(list_del(&list, -2) == LIST_SUCCESS);

    // Delete incorrect positive
    assert(list_del(&list, 5) == LIST_OUT_OF_RANGE);

    // Delete incorrect negative
    assert(list_del(&list, -6) == LIST_OUT_OF_RANGE);

    // Destroy list
    assert(list_destroy(&list) == LIST_SUCCESS);

    // Check if list is NULL
    assert(list == NULL);

    printf("PASS\n");
}

void test_mt_list() {
    printf("%-24s", "Check multi type list: ");

    struct NUMS {
        int field1;
        int field2;
    }nums;

    struct PERSON {
        char name[32];
        int age;
        char gender;
    }person;

    struct UDP {
        uint16_t sport;
        uint16_t dport;
        uint16_t length;
        uint16_t crc;
    }udp;

    void *mt_list = NULL;

    // Add int
    nums.field1 = 0x11111111;
    nums.field2 = 0x22222222;
    assert(mt_list_add(&mt_list, &nums, sizeof(struct NUMS)) == MT_LIST_SUCCESS);

    // Add PERSON
    person.age = 35;
    person.gender = 'm';
    strncpy(person.name, "Aleksei", 32);
    assert(mt_list_add(&mt_list, &person, sizeof(struct PERSON)) == MT_LIST_SUCCESS);

    // Add UDP
    udp.sport = 0xAAAA;
    udp.dport = 0xBBBB;
    udp.length = 0xCCCC;
    udp.crc = 0xDDDD;
    assert(mt_list_add(&mt_list, &udp, sizeof(struct UDP)) == MT_LIST_SUCCESS);

    // Add int
    nums.field1 = 0x33333333;
    nums.field2 = 0x44444444;
    assert(mt_list_add(&mt_list, &nums, sizeof(struct NUMS)) == MT_LIST_SUCCESS);

    // Add int
    nums.field1 = 0x55555555;
    nums.field2 = 0x66666666;
    assert(mt_list_add(&mt_list, &nums, sizeof(struct NUMS)) == MT_LIST_SUCCESS);

    // Insert int before head
    nums.field1 = 0x00000001;
    nums.field2 = 0x00000002;
    assert(mt_list_insert(&mt_list, &nums, sizeof(struct NUMS), 0) == MT_LIST_SUCCESS);

    // Insert NUMS at position 4
    nums.field1 = 0xaaaaaaaa;
    nums.field2 = 0xbbbbbbbb;
    assert(mt_list_insert(&mt_list, &nums, sizeof(struct NUMS), 4) == MT_LIST_SUCCESS);

    // Insert PERSON before tail
    person.age = 42;
    person.gender = 'm';
    strncpy(person.name, "Bob", 32);
    assert(mt_list_insert(&mt_list, &person, sizeof(struct PERSON), -1) == MT_LIST_SUCCESS);

    // Insert incorrect positive
    assert(mt_list_insert(&mt_list, &person, sizeof(struct PERSON), 9) == MT_LIST_OUT_OF_RANGE);

    // Insert incorrect negative
    assert(mt_list_insert(&mt_list, &person, sizeof(struct PERSON), -9) == MT_LIST_OUT_OF_RANGE);

    // Get PERSON
    struct PERSON get_person;
    assert(mt_list_get(&mt_list, 2, &get_person, sizeof(get_person)) == MT_LIST_SUCCESS);
    assert(get_person.age == 35 && get_person.gender == 'm' && strcmp(get_person.name, "Aleksei") == 0);

    // Get NUMS
    struct NUMS get_nums;
    assert(mt_list_get(&mt_list, 4, &get_nums, sizeof(get_nums)) == MT_LIST_SUCCESS);
    assert(get_nums.field1 == 0xaaaaaaaa && get_nums.field2 == 0xbbbbbbbb);

    // Get UDP
    struct UDP get_udp;
    assert(mt_list_get(&mt_list, 3, &get_udp, sizeof(get_udp)) == MT_LIST_SUCCESS);
    assert(get_udp.sport == 0xAAAA && get_udp.dport == 0xBBBB && get_udp.length == 0xCCCC && get_udp.crc == 0xDDDD);

    // Get PERSON before tail
    assert(mt_list_get(&mt_list, -2, &get_person, sizeof(get_person)) == MT_LIST_SUCCESS);
    assert(strcmp(get_person.name,"Bob") == 0 && get_person.gender == 'm' && get_person.age == 42);

    // Get NUMS from tail
    assert(mt_list_get(&mt_list, -1, &get_nums, sizeof(get_nums)) == MT_LIST_SUCCESS);
    assert(get_nums.field1 == 0x55555555 && get_nums.field2 == 0x66666666);

    // Get out of range positive
    assert(mt_list_get(&mt_list, 10, &get_nums, sizeof(get_nums)) == MT_LIST_OUT_OF_RANGE);

    // Get out of range negative
    assert(mt_list_get(&mt_list, -10, &get_nums, sizeof(get_nums)) == MT_LIST_OUT_OF_RANGE);

    // Delete head
    assert(mt_list_del(&mt_list, 0) == MT_LIST_SUCCESS);

    // Delete tail
    assert(mt_list_del(&mt_list, -1) == MT_LIST_SUCCESS);

    // Delete center
    assert(mt_list_del(&mt_list, 3) == MT_LIST_SUCCESS);

    // Delete after head
    assert(mt_list_del(&mt_list, 1) == MT_LIST_SUCCESS);

    // Delete before tail
    assert(mt_list_del(&mt_list, -2) == MT_LIST_SUCCESS);

    // Delete incorrect positive
    assert(mt_list_del(&mt_list, 5) == MT_LIST_OUT_OF_RANGE);

    // Delete incorrect negative
    assert(mt_list_del(&mt_list, -6) == MT_LIST_OUT_OF_RANGE);

    // Destroy list
    assert(mt_list_destroy(&mt_list) == MT_LIST_SUCCESS);

    // Check if list is empty
    assert(mt_list == NULL);

    printf("PASS\n");
}

void test_mt_stack() {
    printf("%-24s", "Check multi type stack: ");

    struct PERSON1 {
        char name[15];
        uint8_t age;
    }person1;

    // Create stack and push int object
    int i = 0x11111111;
    void *mtstack = NULL;
    assert(mt_stack_push(&mtstack, &i, sizeof(i)) == STACK_SUCCESS);

    // Push int object
    i = 0x22222222;
    assert(mt_stack_push(&mtstack, &i, sizeof(i)) == STACK_SUCCESS);

    // Trigger realloc to change pointer
    void *p = malloc(1024);
    assert(p != NULL);

    // Push PERSON object
    person1.age = 35;
    strncpy(person1.name, "Aleksei", sizeof(person1.name));
    assert(mt_stack_push(&mtstack, &person1, sizeof(struct PERSON1)) == STACK_SUCCESS);

    // Push int object
    i = 0x33333333;
    assert(mt_stack_push(&mtstack, &i, sizeof(i)) == STACK_SUCCESS);

    // Check length
    assert(mt_stack_length(&mtstack) == 4);

    // Push uint16_t object
    uint16_t w = 0x4444;
    assert(mt_stack_push(&mtstack, &w, sizeof(w)) == STACK_SUCCESS);

    // Push double object
    double d = 3.14159265;
    assert(mt_stack_push(&mtstack, &d, sizeof(d)) == STACK_SUCCESS);

    // Push int object
    i = 0x55555555;
    assert(mt_stack_push(&mtstack, &i, sizeof(i)) == STACK_SUCCESS);

    // Check length
    assert(mt_stack_length(&mtstack) == 7);

    // Pop int object
    memset(&i, 0, sizeof(i));
    assert(mt_stack_pop(&mtstack, &i, sizeof(i)) == STACK_SUCCESS);
    assert(i == 0x55555555);

    // Pop double object
    memset(&d, 0, sizeof(d));
    assert(mt_stack_pop(&mtstack, &d, sizeof(d)) == STACK_SUCCESS);
    assert(d == 3.14159265);

    // Pop uint16_t object
    memset(&w, 0, sizeof(w));
    assert(mt_stack_pop(&mtstack, &w, sizeof(w)) == STACK_SUCCESS);
    assert(w == 0x4444);

    // Pop int object
    memset(&i, 0, sizeof(i));
    assert(mt_stack_pop(&mtstack, &i, sizeof(i)) == STACK_SUCCESS);
    assert(i == 0x33333333);

    // Check length
    assert(mt_stack_length(&mtstack) == 3);

    // Pop PERSON object
    memset(&person1, 0, sizeof(person1));
    assert(mt_stack_pop(&mtstack, &person1, sizeof(struct PERSON1)) == STACK_SUCCESS);
    assert(person1.age == 35);
    assert(strcmp(person1.name, "Aleksei") == 0);

    // Pop int object
    memset(&i, 0, sizeof(i));
    assert(mt_stack_pop(&mtstack, &i, sizeof(i)) == STACK_SUCCESS);
    assert(i == 0x22222222);

    // Pop incorrect object
    memset(&i, 0, sizeof(i));
    assert(mt_stack_pop(&mtstack, &i, sizeof(struct PERSON1)) == STACK_SIZE_ERROR);
    assert(i == 0);

    // Pop int object
    memset(&i, 0, sizeof(i));
    assert(mt_stack_pop(&mtstack, &i, sizeof(i)) == STACK_SUCCESS);
    assert(i == 0x11111111);

    // Pop from empty stack
    assert(mt_stack_pop(&mtstack, &i, sizeof(i)) == STACK_IS_EMPTY);

    // Check if stack is empty
    assert(mtstack == NULL);

    // Check empty stack length
    assert(mt_stack_length(&mtstack) == 0);

    free(p);
    printf("PASS\n");
}

void test_stack() {
    printf("%-24s", "Check stack: ");

    struct STACK *stack = NULL;
    struct PERSON1 {
        char name[15];
        uint8_t age;
    }person1;

    // Push at uninitialized stack
    person1.age = 32;
    strncpy(person1.name, "Alice", sizeof(person1.name));
    assert(stack_push(stack, &person1) == STACK_ERROR);

    // Pop from uninitialized stack
    assert(stack_pop(stack, &person1) == STACK_ERROR);

    // Initialize stack
    stack = stack_init(STACK_SIZE, sizeof(struct PERSON1));
    assert(stack != NULL);

    // Push object at stack
    person1.age = 35;
    strncpy(person1.name, "Alex", sizeof(person1.name));
    assert(stack_push(stack, &person1) == 3);

    // Push object at stack
    person1.age = 30;
    strncpy(person1.name, "Sam", sizeof(person1.name));
    assert(stack_push(stack, &person1) == 2);

    // Push object at stack
    person1.age = 50;
    strncpy(person1.name, "Bob", sizeof(person1.name));
    assert(stack_push(stack, &person1) == 1);

    // Push object at stack
    person1.age = 15;
    strncpy(person1.name, "Julie", sizeof(person1.name));
    assert(stack_push(stack, &person1) == 0);

    // Change stack size
    assert(stack_reinit(stack, STACK_SIZE + 1) > 0 );

    // Pop last pushed object
    assert(stack_pop(stack, &person1) == 2);
    assert(person1.age == 15);
    assert(strcmp(person1.name, "Julie") == 0);

    // Push object at stack
    person1.age = 15;
    strncpy(person1.name, "Julie", sizeof(person1.name));
    assert(stack_push(stack, &person1) == 1);

    assert(stack_reinit(stack, STACK_SIZE) > 0 );

    // Push object at full stack
    person1.age = 76;
    strncpy(person1.name, "Joe", sizeof(person1.name));
    assert(stack_push(stack, &person1) == STACK_IS_FULL);

    // Pop object from stack
    assert(stack_pop(stack, &person1) == 1);
    assert(person1.age == 15);
    assert(strcmp(person1.name, "Julie") == 0);

    // Push object at stack
    person1.age = 76;
    strncpy(person1.name, "Joe", sizeof(person1.name));
    assert(stack_push(stack, &person1) == 0);

    // Change stack size
    assert(stack_reinit(stack, STACK_SIZE - 1) > 0);

    // Pop object from stack
    assert(stack_pop(stack, &person1) == 1);
    assert(person1.age == 50);
    assert(strcmp(person1.name, "Bob") == 0);

    // Pop object from stack
    assert(stack_pop(stack, &person1) == 2);
    assert(person1.age == 30);
    assert(strcmp(person1.name, "Sam") == 0);

    // Pop object from stack
    assert(stack_pop(stack, &person1) == 3);
    assert(person1.age == 35);
    assert(strcmp(person1.name, "Alex") == 0);

    // Pop object from empty stack
    assert(stack_pop(stack, &person1) == STACK_IS_EMPTY);

    // Destroy stack
    assert(stack_del(stack) == STACK_SUCCESS);

    printf("PASS\n");
}

void test_map() {
    printf("%-24s", "Check map: ");
    int value;

    int *values = malloc(MAP_ENTRIES * sizeof(int));
    if(values == NULL) {
        printf("Can't allocate memory for values\n");
        return;
    }
    char *keys = malloc(MAP_ENTRIES * 10);
    if(values == NULL) {
        free(values);
        printf("Can't allocate memory for keys\n");
        return;
    }
    memset(values, 0, sizeof(int) * MAP_ENTRIES);
    memset(keys, 0, MAP_ENTRIES * 10);

    struct MAP map = {.objects = NULL};
    assert(map_init(&map, 150000) == MAP_OK);

    // Fill keys and values arrays, add them into the map
    for(int i = 0; i != MAP_ENTRIES; ++i) {
        values[i] = i;
        sprintf(&keys[i * 10], "key%i", i);
        assert(map_add(&map, &keys[i * 10], 10, &values[i], sizeof(int)) == MAP_OK);
    }

    // Check number of entries in the map
    assert(map.count == MAP_ENTRIES);

    // Check that each value matches the key
    for(int i = 0; i != MAP_ENTRIES; ++i) {
        value = -1;
        assert(map_get(&map, &keys[i * 10], 10, &value, sizeof(value)) == sizeof(value));
        assert(value == values[i]);
    }

    // Check invalid value size
    assert(map_get(&map, &keys[49999 * 10], 10, &value, 2) == MAP_VALUE_ERROR);

    // Check value changing
    value = -1;
    assert(map_add(&map, &keys[49999 * 10], 10, &value, sizeof(int)) == MAP_OK);
    value = 0;
    assert(map_get(&map, &keys[49999 * 10], 10, &value, sizeof(value)) == sizeof(value));
    assert(value == -1);

    // Check value size changing
    char hello[] = "Hello, World!";
    assert(map_add(&map, &keys[49998 * 10], 10, hello, sizeof(hello)) == MAP_OK);
    memset(hello, 0, sizeof(hello));
    assert(map_get(&map, &keys[49998 * 10], 10, hello, sizeof(hello)) == sizeof(hello));
    assert(memcmp(hello, "Hello, World!", sizeof(hello)) == 0);

    // Check iteration by the map
    assert(map_get_objects_start(&map) == MAP_OK);
    int i = 0;
    while(map_get_objects_next(&map)) {
        ++i;
    }
    assert(i == MAP_ENTRIES);

    // Check key deleting
    for(int i = 0; i != MAP_ENTRIES; ++i) {
        assert(map_del(&map, &keys[i * 10], 10) == MAP_OK);
    }
    assert(map.count == 0);

    // Destroy the map
    assert(map_destroy(&map) == MAP_OK);
    assert(map.count == 0 && map.length == 0 && map.objects == NULL);

    free(keys);
    free(values);

    printf("PASS\n");
}

void test_array() {
    printf("%-24s", "Check array: ");

    // Init array
    struct ARRAY a;
    assert(array_init(&a) == 0);

    // Add objects to the array
    for(int i = 0; i < ARRAY_SIZE; ++i) {
        assert(array_append(&a, &i, sizeof(i)) == 0);
    }

    // Check objects
    assert(array_get_objects_start(&a, 0) == 0);
    struct ARRAY_ENTRY *e;
    for(int i = 0; i < ARRAY_SIZE; ++i) {
        assert((e = array_get_objects_next(&a)) != NULL);
        assert(*(int *)e->data == i);
    }
    assert(array_len(&a) == ARRAY_SIZE);

    // Check end of array
    assert(array_get_objects_next(&a) == NULL);

    // Put an object at the index ARRA
    int n = 255;
    assert(array_put(&a, &n, sizeof(n), ARRAY_SIZE / 4) == 0);
    // Get and check changed object
    n = 0;
    assert(array_get(&a, ARRAY_SIZE / 4, &n, sizeof(n)) == 0);
    assert(n == 255);

    // Put object into the array at invalid index
    n = ARRAY_SIZE;
    assert(array_put(&a, &n, sizeof(n), ARRAY_SIZE) == ARRAY_INVALID_INDEX);

    // Test the object at invalid index
    assert(array_get(&a, ARRAY_SIZE, &n, sizeof(n)) == ARRAY_INVALID_INDEX);

    // Delete slice at center a[ARRAY_SIZE/4:ARRAY_SIZE*3/4]
    assert(array_del(&a, ARRAY_SIZE / 4, ARRAY_SIZE - ARRAY_SIZE / 4 - 1) == 0);

    // Check if array slice was deleted
    assert(array_get_objects_start(&a, 0) == 0);
    for(int i = 0; i < ARRAY_SIZE / 4; ++i) {
        assert((e = array_get_objects_next(&a)) != NULL);
        assert(*(int *)e->data == i);
    }
    assert(array_get_objects_start(&a, ARRAY_SIZE/4) == 0);
    for(int i = ARRAY_SIZE - ARRAY_SIZE / 4; i < ARRAY_SIZE; ++i) {
        assert((e = array_get_objects_next(&a)) != NULL);
        assert(*(int *)e->data == i);
    }
    assert(array_len(&a) == ARRAY_SIZE / 2);

    // Delete slice at start
    assert(array_del(&a, 0, 9) == 0);
    assert(array_len(&a) == ARRAY_SIZE / 2 - 10);

    // Delete slice from center to end
    assert(array_del(&a, 15, ARRAY_SIZE) == 0);
    assert(array_get_objects_start(&a, 0) == 0);
    for(int i = 10; i < 24; ++i) {
        assert((e = array_get_objects_next(&a)) != NULL);
        assert(*(int *)e->data == i);
    }
    assert(array_len(&a) == 15);

    // Insert at start
    n = 9999;
    assert(array_insert(&a, 0, &n, sizeof(n)) == 0);
    n = 0;
    assert(array_get(&a, 0, &n, sizeof(n)) == 0);
    assert(n == 9999);
    assert(array_len(&a) == 16);

    // Insert at center
    n = 8888;
    assert(array_insert(&a, 8, &n, sizeof(n)) == 0);
    n = 0;
    assert(array_get(&a, 8, &n, sizeof(n)) == 0);
    assert(n == 8888);
    assert(array_len(&a) == 17);

    // Insert at end
    n = 1111;
    assert(array_insert(&a, ARRAY_SIZE, &n, sizeof(n)) == 0);
    n = 0;
    assert(array_get(&a, array_len(&a) - 1, &n, sizeof(n)) == 0);
    assert(n == 1111);
    assert(array_len(&a) == 18);

    // Delete all objects from array
    assert(array_del(&a, 0, ARRAY_SIZE) == 0);
    assert(array_len(&a) == 0);
    assert(array_size(&a) == sizeof(struct ARRAY));

    // Destroy array
    assert(array_destroy(&a) == 0);

    printf("PASS\n");
}

int main(int argc, char const *argv[]) {
    test_map();
    test_stack();
    test_mt_stack();
    test_queue();
    test_list();
    test_mt_list();
    test_array();

    return 0;
}
