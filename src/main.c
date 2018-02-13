//
//  main.c
//  test2
//
//  Created by Rajesh Mallavajula on 2/10/18.
//  Copyright © 2018 Rajesh Mallavajula. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}


int main(int argc, const char * argv[]) {
    
    // --------Initializations --------------
    
    
    char str0[3], str1[1000];
    int percentile, percentile_contribution_no;
    int i=0, k=0,l=0, m=0, rdata_count=0, abc=0,  efg=0, mno=0, iter=0;
    int *sort_contributions;
    //int *raj;
    
    char CMTE_ID[10], NAME1[20], dummy1[10], dummy2[10], dummy3[10], dummy4[10], dummy5[10], dummy6[10], dummy7[10], dummy8[10], dummy9[10], dummy10[10], TRANSACTION_DT[10], TRANSACTION_AMT[10], OTHER_ID[10], ZIP_CODE[10];
    struct data {
        char CMT[10];
        char ZIP[10];
        int YEAR;
        int AMT;
        char NAME[20];
    };
    struct repeat_donor_data{
        char ZIP[10];
        int total_contributions;
        int *contributions;
        int total_no_contributions;
        int YEAR;
    };
    struct data *record1;
    record1=(struct data *) calloc(1, sizeof(struct data));
    
    struct data *repeat1;
    repeat1=(struct data *) calloc(1, sizeof(struct data));
    struct data repeat[20]={0};
    
    struct repeat_donor_data *rdata1;
    rdata1=(struct repeat_donor_data *) calloc(1, sizeof(struct repeat_donor_data));
    rdata1[0].contributions = (int *) calloc(1, sizeof(int));
    rdata1[0].contributions =realloc(rdata1[0].contributions, sizeof(int));
    if (rdata1[0].contributions==NULL) {
        printf("MEMORY COULD NOT BE ALLOCATED");
    }
    
    
    //----------Reading percentile.txt file --------------
    FILE *p;
    p=fopen("../input/percentile.txt", "r");
    fgets(str0, 1000, p);
    percentile = atoi(str0);
    
    // ------ Opening repeat_donors.txt in write mode --------------
    
    FILE *r;
    r= fopen("../output/repeat_donors.txt", "w");
    
    
    //------------Reading itcont.txt file-----------------
    
    FILE *f;
    f=fopen("../input/itcont.txt","r"); //Opening the file from location
    while (fgets(str1, 1000, f)!=NULL) {
        
        //Reading each line of the file seperated by pipe delimiters and storing it in str1
        sscanf(str1, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%*s", CMTE_ID,dummy1, dummy2,dummy3,dummy4,dummy5,dummy6,NAME1,dummy7,dummy8,ZIP_CODE,dummy9, dummy10,TRANSACTION_DT,TRANSACTION_AMT,OTHER_ID);
        
        
       if (strcmp(OTHER_ID, "")==0 && strlen(TRANSACTION_DT)==8 && strlen(ZIP_CODE)>=5 && strcmp(NAME1, "")!=0 && strcmp(CMTE_ID, "")!=0 && strcmp(TRANSACTION_AMT, "")!=0 ) {
           ZIP_CODE[5]=0;
           record1[i].AMT=atoi(TRANSACTION_AMT);
           record1[i].YEAR=atoi(TRANSACTION_DT)%10000;
           strcpy(record1[i].CMT, CMTE_ID);
           strcpy(record1[i].NAME, NAME1);
           strcpy(record1[i].ZIP, ZIP_CODE);
           record1 = realloc(record1, sizeof(struct data));
        
           while (k<i) {

               if (strcmp(record1[i].ZIP, record1[k].ZIP)==0) {
                   if (strcmp(record1[i].NAME,record1[k].NAME)==0){
                       strcpy(repeat1[l].ZIP,record1[i].ZIP);
                       repeat1[l].AMT=record1[i].AMT;
                       repeat1[l].YEAR=record1[i].YEAR;
                       strcpy(repeat1[l].CMT,record1[i].CMT);
                       repeat1 = realloc(repeat1, sizeof(struct data));
                     if (l==0) {
                           strcpy(rdata1[0].ZIP,repeat1[l].ZIP);
                           rdata1[0].YEAR= repeat1[l].YEAR;
                           rdata1[0].total_contributions=repeat1[l].AMT;
                           mno=repeat1[l].AMT;
                         if (rdata1[0].contributions== NULL) {
                             printf("error in allocating memory");
                             exit(0);
                         }
                             
                         *rdata1[0].contributions=mno;
                           rdata1[0].total_no_contributions=1;
                           rdata1 = realloc(rdata1, sizeof(struct repeat_donor_data));
                           rdata1[1].total_contributions=0;
                           rdata1[1].total_no_contributions=0;
                           rdata1[1].contributions=(int *) calloc(1, sizeof(int));
                           rdata1[1].contributions =realloc(rdata1[1].contributions, sizeof(int));
                         fprintf(r, "%s|%s|%d|%d|%d|1\n", repeat1[l].CMT,rdata1[0].ZIP,  rdata1[0].YEAR,repeat1[l].AMT, repeat1[l].AMT);
                       } else {
                           while (m<=rdata_count) {
                               if (strcmp(repeat1[l].ZIP,rdata1[m].ZIP)==0 && repeat1[l].YEAR==rdata1[m].YEAR) {
                                   rdata1[m].total_contributions=rdata1[m].total_contributions+repeat1[l].AMT;
                                   efg=sizeof(*rdata1[m].contributions)/sizeof(int);
                                   rdata1[m].contributions[efg] = repeat1[l].AMT;
                                   rdata1[m].contributions =realloc(rdata1[m].contributions, sizeof(int));
                                   rdata1[m].total_no_contributions=rdata1[m].total_no_contributions+1;
                                   if (((percentile*rdata1[m].total_no_contributions)%100)==0) {
                                       percentile_contribution_no =((percentile*rdata1[m].total_no_contributions)/100);
                                   } else {
                                       percentile_contribution_no =((percentile*rdata1[m].total_no_contributions)/100)+1;
                                   }
                                   sort_contributions = (int *) calloc(efg+1, sizeof(int));
                                   for (iter=0; iter<=efg; iter++) {
                                       sort_contributions[iter]=rdata1[m].contributions[iter];
                                   }
                                   qsort(sort_contributions, efg+1, sizeof(int), cmpfunc);
                                   fprintf(r, "%s|%s|%d|%d|%d|%d\n", repeat1[l].CMT, repeat1[l].ZIP, repeat1[l].YEAR, sort_contributions[percentile_contribution_no-1], rdata1[m].total_contributions, rdata1[m].total_no_contributions );
                                   m=0;
                                   break;
                               }
                               m++;
                           }
                           if (m!=0) {
                               rdata_count++;
                               strcpy(rdata1[rdata_count].ZIP,repeat[l].ZIP);
                               rdata1[rdata_count].total_contributions=repeat[l].AMT;
                               rdata1[rdata_count].YEAR=repeat1[l].YEAR;
                               rdata1[rdata_count].contributions[0]=repeat[l].AMT;
                               rdata1[rdata_count].contributions =realloc(rdata1[rdata_count].contributions, sizeof(int));
                               rdata1[rdata_count].total_no_contributions=1;
                               rdata1 = realloc(rdata1, sizeof(struct repeat_donor_data));
                               rdata1[rdata_count+1].contributions=(int *) calloc(1, sizeof(int));
                               fprintf(r, "%s|%s|%d|%d|%d|1\n", repeat1[l].CMT,repeat1[l].ZIP, repeat1[l].YEAR,repeat1[l].AMT, repeat1[l].AMT);
                           }
                       }
                       
                       l++;
                       break;
                   }
                   
               }
               k++;
           }
           k=0;
           i++;
        }
        
        strcpy(OTHER_ID, "");
    }
    
    fclose(f);
    
    while(abc<l){
        printf("RepeatCMTE_ID: %s\n", repeat1[abc].CMT);
        printf("RepeatZIP: %s\n", repeat1[abc].ZIP);
        printf("RepeatAMT: %d\n", repeat1[abc].AMT);
        printf("RepeatYEAR: %d\n", repeat1[abc].YEAR);
        abc++;
    }
    
    return 0;
}

