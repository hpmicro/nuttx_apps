/****************************************************************************
 * apps/examples/fpu_test/fpu_test_main.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <nuttx/lib/lib.h>

#define CONFIG_WHILE1_PRIORITY 103
#define CONFIG_WHILE1_STACKSIZE 2048

#define CONFIG_WHILE2_PRIORITY 103
#define CONFIG_WHILE2_STACKSIZE 2048

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * fpu_test_main
 ****************************************************************************/
static double cal_pai(void)
{
  double r =0;
  double sum =0;
  uint32_t i =0;
  for(i=0;i<1000000;i++)
  {

    r = ((i & 1) ? (double)-1.0: (double)1.0)/(2*i+1);
    sum = sum + r;
  }
  return sum * 4.0;
}

static int while1_entry(int argc, char *argv[])
{
  double pi =0;
  uint32_t count =0;
  printf("Enter while task1!\n");
  while(1)
  {
    count ++;
    pi = cal_pai();
    printf("While task1! count : %ld,pi = %.10lf\n",count,pi);
  }
  return 0;
}

static int while2_entry(int argc, char *argv[])
{
  double pi =0;
  uint32_t count =0;
  printf("Enter while task2!\n");
  while(1)
  {
    count ++;
    pi = cal_pai();
    printf("While task2! count : %ld,pi = %.10lf\n",count,pi);
  }
  return 0;
}

int main(int argc, FAR char *argv[])
{
  uint32_t count =0;
  double pi =0;
  int ret;
  uint32_t random_time =0;

  printf("ENTER FPU Test!!\n");

  printf("Create 2 while task!!\n");

  ret = task_create("while1_entry", CONFIG_WHILE1_PRIORITY,
                     CONFIG_WHILE1_STACKSIZE,
                      while1_entry, NULL);

  if(ret < 0)
  {
    int errcode = errno;
    printf("while task1 create failed, reason: %d!!\n",errcode);
  }
  else
  {
    printf("while task1 create successfully\n");
  }

  ret = task_create("while2_entry", CONFIG_WHILE2_PRIORITY,
                     CONFIG_WHILE2_STACKSIZE,
                      while2_entry, NULL);

  if(ret < 0)
  {
    int errcode = errno;
    printf("lwhile task2 create failed, reason: %d!!\n",errcode);
  }
  else
  {
    printf("while task2 create successfully\n");
  }

  while(1)
  {
    count ++;

    random_time = (uint32_t)nrand(5000);

    pi = cal_pai();
    printf("main_task :count = %ld,pi = %.10lf\n", count,pi);
    printf("Sleep time: %ld ms\n",random_time);
    usleep(random_time*1000);
  }

  printf("Exit FPU Test!!\n");
  return 0;
}
