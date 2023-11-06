#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <pwd.h>

#include <errno.h>

#include "utest.h"

#define MAX_PASSWD_BUFLEN 65536

void print_passwd(struct passwd const *p)
{
    if (p == NULL) return;

    printf("[Name=%s, Uid=%d, Gid=%d, Comment=%s, Home=%s, Shell=%s]\n",
           p->pw_name,
           p->pw_uid,
           p->pw_gid,
           p->pw_gecos,
           p->pw_dir,
           p->pw_shell);
}

UTEST(GET_USER, LEGACY)
{
    struct passwd *p_root = getpwnam("root");
    ASSERT_NE(p_root, NULL);

    ASSERT_STREQ("root", p_root->pw_name);
    ASSERT_EQ(0, p_root->pw_uid);
    print_passwd(p_root);

    struct passwd *p_user = getpwuid(1000);
    ASSERT_EQ(1000, p_user->pw_uid);
    print_passwd(p_user);
}

UTEST(GET_PASSWD, REENTRANT)
{
    bool unlikely_case = false;
    long buflen = sysconf(_SC_GETPW_R_SIZE_MAX); // retrieve buffer length
    if (buflen == -1) {
        buflen = MAX_PASSWD_BUFLEN;
    }

    char *buf_nam = (char*)malloc(buflen);
    EXPECT_NE(buf_nam, NULL);

    // meant to be the same resource
    struct passwd nam_pwd = { 0 };
    struct passwd *nam_result = NULL;

    int nam_status = getpwnam_r("root", &nam_pwd, buf_nam, buflen, &nam_result);
    if (nam_status != 0) {
        if (nam_status == ERANGE) {
            buflen = MAX_PASSWD_BUFLEN;
            buf_nam = (char*)realloc(buf_nam, buflen); // enlarge the buffer and try again
            nam_status = getpwnam_r("root", &nam_pwd, buf_nam, buflen, &nam_result);

            ASSERT_EQ(0, nam_status); // no way
        } else {
            unlikely_case = true;
            goto buf_name_cleanup;
        }
   }
   ASSERT_EQ(&nam_pwd, nam_result);
   nam_result = NULL; // one owner principle
   ASSERT_STREQ("root", nam_pwd.pw_name);
   ASSERT_EQ(0, nam_pwd.pw_uid);
   print_passwd(&nam_pwd);


   char *buf_uid = (char*)malloc(buflen);
   EXPECT_NE(buf_uid, NULL);

   struct passwd uid_pwd = { 0 };
   struct passwd *uid_result = NULL;
   int uid_status = getpwuid_r(1000, &uid_pwd, buf_uid, buflen, &uid_result);
   if (uid_status != 0) {
       if (uid_status == ERANGE) { // in case the buffer is not large enough
            buflen = MAX_PASSWD_BUFLEN;
            buf_uid = (char*)realloc(buf_uid, buflen);
            uid_status = getpwuid_r(1000, &uid_pwd, buf_uid, buflen, &uid_result);

            ASSERT_EQ(0, uid_status);
        } else {
            unlikely_case = true;
            goto buf_uid_cleanup;
        }
   }
   ASSERT_EQ(&uid_pwd, uid_result);
   uid_result = NULL;
   ASSERT_EQ(1000, uid_pwd.pw_uid);
   print_passwd(&uid_pwd);

buf_uid_cleanup:
    free(buf_uid);
    buf_uid = NULL;

buf_name_cleanup:
    free(buf_nam);
    buf_nam = NULL;

unlikely:
    if (unlikely_case) {
        EXPECT_TRUE_MSG(false, "no way this could fail");
    }
}


UTEST_MAIN();
