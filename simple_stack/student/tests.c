// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

#include <string.h>

void print_array_as_stack(char **array, int l){
  int i;
  for (i=l-1; i>=0; i-- ){
    printf("array as stack %s ", *(array+i));
  }
}

void print_stack(struct node *head, int l){
  if (!head){
    printf("NO stack");
  }

  int i;
  struct node *run = head;
  printf("head:  ");
  for (i =0; i<l; i++){
    printf("%s \t| \tv ", run->name);
    run = run->next;
  }
  printf("\tNULL ");
}

int check_stack(struct node *head, char** array, int len){
  if (!head){
    if (!array && len == 0)
      return 0;
    else
      return 1;
  }

  // check if the structure of the stack correspond to array
  int i, l = len-1;
  struct node *run = head;
  char *src, *st;

  for (i = 0; i < l; i++){
    src = *(array+l-i);
    st = run->name;

    //printf("array = %s, name = %s ", src, st);
    if (!run || strcmp(src, st) != 0){
      return 1;
    }
    run = run->next;
  }

  return 0;
}

void free_stack(struct node *head){
  if (!head)
    return;

  struct node *run = head, *tmp;
  while (!run){
    tmp = run;
    run = run->next;
    free(tmp->name);
    free(tmp);
  }
  free(head);
}

struct node *generate_stack(char **array, int l){
  struct node *head = NULL;
  char * str;
  int i, len;

  for (i=0; i<l; i++){
    str = *(array+i);
    len = strlen(str) + 1;
    char *name = (char*) malloc(len);
    if (!name)
      return NULL;

    struct node *node = (struct node*) malloc(sizeof(struct node));
    if (!node)
      return NULL;

    strncpy(name, str, len);
    node->name = name;
    node->next = head;

    //printf("stack = %s ", name);

    head = node;

  }

  return head;
}

/* Check the behavior of the function if the given @head is NULL or if the malloc call fails
*/
void test_push_param_nomem() {
  set_test_metadata("push", _("Check the behavior of the function if the given @head is NULL or if the malloc call fails"), 1);

  int ret;

  SANDBOX_BEGIN;
  ret = push(NULL, "test");
  SANDBOX_END;

  // check return value when null arg
  CU_ASSERT_EQUAL(1, ret);
  if (!ret)
      push_info_msg(_("Wrong return code if wrong args"));

  // check the return value if malloc call fails
  struct node *head = (struct node*) malloc(sizeof(struct node));
  /*if (!head)
    return;*/
  head->name = "test";
  head->next = NULL;

  monitored.malloc = true;
  failures.malloc = FAIL_ALWAYS;
  failures.malloc_ret = NULL;

  SANDBOX_BEGIN;
  ret = push(&head, "ksdhfsfqsdfh");
  SANDBOX_END;

  CU_ASSERT_EQUAL(1, ret);
  if (!ret)
     push_info_msg(_("Wrong return code if wrong args"));

  free(head);
}

void test_push_changing_param(){
  set_test_metadata("push", _("Check if the function change @value"), 1);

  //int ret;

  struct node *head = (struct node*) malloc(sizeof(struct node));
  /*if (!head)
    return;*/
  head->name = "test";
  head->next = NULL;

  char *src = "ksdhfsfqsdfh";
  int len = strlen(src);

  char *src_cpy = (char*) malloc(len);
  /*if (!src_cpy)
    return;*/

  strncpy(src_cpy, src, len+1);

  SANDBOX_BEGIN;
  push(&head, src);
  SANDBOX_END;
  //printf("src = %s, src_cpy = %s ", src, src_cpy);
  int cmp = strcmp((const char*) src, (const char*) src_cpy);
  CU_ASSERT_TRUE(!cmp);
  if (cmp != 0)
    push_info_msg(_("The function changed the value of @value"));

  free_stack(head);
}

void test_push_general() {
  set_test_metadata("push", _("Check if the insertion of the new node is correct"), 1);

  char *a[7] = {"fqgrsrfgfg", "moty;oe26rbgs",
  "i;rvqr6tgbsecr26", "5qvf15rg5g", "42",
   "r157g1srq7v16zs6", ""};

  struct node *stack = generate_stack(a, 6);
  if (!stack)
    return;

  int ret;

  char *src ="e5f1vq6f8e81E";

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = push(&stack, src);
  SANDBOX_END;

  // check if only one call to malloc
  int ms = stats.malloc.called;
  CU_ASSERT_EQUAL(ms, 2);
  if (ms != 2)
    push_info_msg(_("You can use only two call to malloc to complete the task"));

  // check if new element is malloced
  int mal = malloced((void*) stack);
  CU_ASSERT_TRUE(mal);
  // if malloced, check the value, else not because it produces buffer overflow due to CUNIT
  if (mal){
    //printf("src = %s, name = %s ", src, stack->name);
    CU_ASSERT_STRING_EQUAL(src, stack->name);
    if (!strcmp(src, stack->name)){
      char tmp[100];
      sprintf(tmp, _("The pushed value differ from the expected one Waited : %s Received : %s "), src, stack->name);
      push_info_msg(tmp);
    }

  }
  else
    push_info_msg(_("The returned pointer is not malloced"));

  if (!stack && !stack->next){
    CU_FAIL("The head node is not correctly updated");
  }
  else{
    // add src to the array stack
    *(a+6) = src;

    //print_array_as_stack(a ,7);
    //print_stack(stack , 7);

    // if correct struct, @stru = 0
    int stru = check_stack(stack, a, 7);
    CU_ASSERT_TRUE(!stru);
    if (stru)
      push_info_msg(_("The structure of the stack changed or is not the expected one"));

    // check the return value of the function
    CU_ASSERT_TRUE(!ret);
    if (ret)
      push_info_msg(_("The function return an error code in normal case"));
      free_stack(stack);
  }
}

void test_pop_args(){
    set_test_metadata("pop", _("Check the behavior of the function when passing wrong args"), 1);

    int ret;
    char *result = NULL;

    SANDBOX_BEGIN;
    ret = pop(NULL, &result);
    SANDBOX_END;

    CU_ASSERT_TRUE(ret);
    if (!ret)
      push_info_msg(_("Wrong return value when passing wrong args "));
}

void test_pop_value(){
  set_test_metadata("pop", _("Check the behavior of the function in normal case"), 1);

  int ret;
  char *result = NULL;

  char *a[6] = {"fqgrsrfgfg", "moty;oe26rbgs",
  "i;rvqr6tgbsecr26", "5qvf15rg5g", "42",
   "r157g1srq7v16zs6"};

  struct node *stack, *tmp_head;

  //print_stack(stack, 6);

  monitored.malloc = true;

  SANDBOX_BEGIN;
  stack =  generate_stack(a, 6);
  tmp_head = stack;
  ret = pop(&stack, &result);
  SANDBOX_END;

  //print_stack(stack, 5);

  // check the return value
  CU_ASSERT_TRUE(!ret);
  if (ret)
    push_info_msg(_("The function returns the wrong value "));

  // check the popped value
  char *src = *(a+5);
  //printf("src = %s, result = %s, size = %li ", src, result, strlen(src));
  //CU_ASSERT_STRING_EQUAL(src, result);
  int cmp = strncmp((const char*) result, (const char*) src, strlen(src));
  CU_ASSERT_TRUE(!cmp);
  if (cmp)
    push_info_msg(_("The function popped the wrong value "));

  // check head of the stack
  char *src2 = *(a+4);
  //printf("src = %s, result = %s, size = %li ", src2, stack->name, strlen(stack->name));
  int cmp2 = strncmp((const char*) stack->name, (const char*) src2, strlen(src2));
  CU_ASSERT_TRUE(!cmp2);
  if (cmp2)
    push_info_msg(_("The head is not the waited one "));

  // check if the function free the first node (not the string inside !)
  int ml = malloced(tmp_head);
  //printf("%i ", ml);
  CU_ASSERT_TRUE(!ml);
  if (ml)
    push_info_msg(_("The previous head is not freed "));


  // check the structure if the stack after popped
  if (!stack && !stack->next){
    CU_FAIL("The head node is not correctly updated");
  }
  else{
    // if correct struct, @stru = 0
    int stru = check_stack(stack, a, 5);
    CU_ASSERT_TRUE(!stru);
    if (stru)
      push_info_msg(_("The structure of the stack changed or is not the expected one"));

    // check the return value of the function
    CU_ASSERT_TRUE(!ret);
    if (ret)
      push_info_msg(_("The function return an error code in normal case"));
      //free_stack(stack);
  }
}

/**
*
*/
void test_pop_empty(){
  set_test_metadata("pop", _("Check the behavior of the function when popping last elemennt"), 1);

  struct node *head = (struct node*) malloc(sizeof(struct node));
  head->name = (char*) malloc(10);
  char* src = "vuoeznrugh";
  strncpy(head->name, src, 10);

  head->next = NULL;
  char *dst;
  int ret;

  monitored.malloc = true;

  SANDBOX_BEGIN;
  ret = pop(&head, &dst);
  SANDBOX_END;

  CU_ASSERT_TRUE(!ret);
  if (ret)
    push_info_msg(_("The function return an error code in normal case"));

  int ml = malloced(head);
  CU_ASSERT_TRUE(!ml);
  if (ml)
    push_info_msg(_("The previous head is not freed "));
}

/*void test_push_empty(){
  set_test_metadata("push", _("Check if the insertion of a new node into an empty list"), 1);

  struct node *head = (struct node*) malloc(sizeof(struct node));
  if (!head)
    return;

  head->next = NULL;
  head->value = 0;

 int ret;

 srandom(646415654);

 SANDBOX_BEGIN;
 ret = push(&head, random());
 SANDBOX_END;

 CU_ASSERT_EQUAL(ret, 0);
 if (!ret)
  push_info_msg(_("You detected an error in normal case"));



  free_stack(head);
}*/

int main(int argc,char** argv)
{
    BAN_FUNCS();
    // warning, test_push_general return and kill other tests
    // TODO: find why
    RUN( test_pop_value, test_push_param_nomem, test_push_changing_param, test_pop_args, test_pop_empty, test_push_general);
}
