#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "list.h"
#include "list_multitype.h"
#include "stack.h"
#include "queue.h"

#define STACK_SIZE 4

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

    printf("PASS\n");
}

int main(int argc, char const *argv[]) {
    test_stack();
    test_mt_stack();
    test_queue();
    test_list();
    test_mt_list();

    return 0;
}
