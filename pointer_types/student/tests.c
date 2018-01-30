// CTester template

#include <stdlib.h>
#include "student_code.h"
#include "CTester/CTester.h"

int sol1(int* a){
    return a[2];
}

int sol2(int* a){
    char* c = (char*) a;
    return  (int) *(c + 4);
}

int sol3(int* a){
    char* c = (char*) a;
    c = c + 12;
    int * d = (int*) c;
    return *d;
}

void test_tab1() {
    set_test_metadata("First input", _("test for the first input"), 1);

    int tab1[4] = {2, 3 , 5, 9};
    int* a = (int*) tab1;
    int ret = 0;

    SANDBOX_BEGIN;
    ret = first(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,5);

    SANDBOX_BEGIN;
    ret = second(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,3);

    SANDBOX_BEGIN;
    ret = third(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,9);
}

void test_tab2() {
    set_test_metadata("First input", _("test for the second input"), 1);

    int tab2[4] = {1, 4, 4, 10};
    int * a = (int*) tab2;

    int ret = 0;

    SANDBOX_BEGIN;
    ret = first(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,4);

    SANDBOX_BEGIN;
    ret = second(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,4);

    SANDBOX_BEGIN;
    ret = third(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,10);
}

void test_tab3() {
    set_test_metadata("First input", _("test for the third input"), 1);

    int tab3[4] = {10, 4, 21, 3};
    int * a = (int*) tab3;

    int ret = 0;

    SANDBOX_BEGIN;
    ret = first(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,21);

    SANDBOX_BEGIN;
    ret = second(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,4);

    SANDBOX_BEGIN;
    ret = third(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,3);
}

void test_tab4() {
    set_test_metadata("First input", _("test for the fourth input"), 1);

    int tab4[4] = {10, 456789087, 2121212122, 672386787};
    int * a = (int*) tab4;

    int ret = 0;

    SANDBOX_BEGIN;
    ret = first(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,2121212122);

    SANDBOX_BEGIN;
    ret = second(a);
    SANDBOX_END;

    CU_ASSERT_EQUAL(ret,95);

    SANDBOX_BEGIN;
    ret = third(a);
    SANDBOX_END;
    
    printf("%d\n",sol3(a));

    CU_ASSERT_EQUAL(ret,sol3(a));
}

int main(int argc,char** argv)
{
    BAN_FUNCS();
    RUN(test_tab1,test_tab2,test_tab3,test_tab4);
}
