#include<stdio.h>
#include<iostream.h>
#include<stdlib.h>
#include<fstream.h>
#include<string.h>
#include <iomanip.h>
#include<math.h>

struct eventlog{
	char event[30];
	float weight;
}el[20];

struct base{
	float events[20];
}ba[20];

struct test{
	char line[30];
	float events[20];
}te[20];


struct calc{
	char event[30];
	float avg;
	float sd;
	float weight;
}cl[20];


float concat(float v, float a)
{
	float b = (v*10) + a;
	return b;
}

int count_n(int v, int a)
{
	int b = (v*10) + a;
	return b;
}

int get_number(char line[], int size)
{
int v = 0;
	for (int s=0;s<size-2;s++)
	{
		int a = line[s] - '0';
		//cout << "a: " << a<< endl;
		v = count_n(v,a);
	}
//cout << " NUmber of lines are " << v << endl;
return v;
}


int main()
{


FILE *sfile;
char line[100];
sfile = fopen("events.txt","r");
//cout << "here we are" << endl;
int no=0; int flag=1; int count = 0; float div = 1; int flag1=0;
while(fgets(line, sizeof(line), sfile) != NULL)
{
	//cout << "Total Number is " << no << endl;
	if(flag==1)
	{
				
		no = get_number(line, strlen(line));
		flag = 0;
		//cout << "Total Number from first line is " << no << endl;
	}
	else
	{
		if (no>=21)
		{
			cout << "The Number of monitored events should be less than 20" <<endl;
			exit(0);
		}
		else
		{
			int parity = 1; int v = 0; float j=0;
			for (int s=0;s<strlen(line);s++)
			{
				int val = line[s];
				if(val == 58)
				{
					if(parity==1)
					{	//cout << "Event: " << el[count].event << endl;
						parity = 2; v=0;
					}
					else
					{
						parity = 1; el[count].weight = j; j=0; v=0; div=1;flag1=0;
						//cout << "Weight: " << el[count].weight << endl;
						count++;
						//getch();
					}
				}
				else
				{
					if(parity==1)
					{
						el[count].event[v]=line[s];
						v++;
					}
					else
					{
						if (val==46)
						{
							flag1=1;
						}
						else if (flag1==1)
						{
							div = div * 10;
							float h = line[s] - '0';
							h = h/div;
							j = j+h;
						}
						else
						{
							float h = line[s] - '0';
							j = concat(j,h);
						}
					}
				}
			}
		}
	}
}
fclose(sfile);
cout << "The total Event count is " << count<< endl;
cout << endl;

int base_count = 0; //number of lines in base file
int be_count = 0;  //number of events in base file
sfile = fopen("base.txt","r");
while(fgets(line, sizeof(line), sfile) != NULL)
{
	float j=0; int n=0;
	for (int s=0;s<strlen(line);s++)
	{
		int val = line[s];
		if(val == 58)
		{
			ba[base_count].events[n] = j;
			n=n+1;
		       j=0;
		       be_count = n;
		}
		else
		{
			float h = line[s] - '0';
			j = concat(j,h);
		}

	}
n=0;
base_count = base_count+1;
}
fclose(sfile);

/*
cout << "The total events in base files are : " << base_count << endl;
cout << "The total number of events in each array : " << be_count << endl;

for(int g=0;g<base_count;g++)
{
	for(int i=0;i<be_count;i++)
	{
		cout <<  ba[g].events[i] << "\t";
	}
	cout << endl;
}
*/

int test_count = 0; //number of lines in base file
int te_count = 0;  //number of events in base file
sfile = fopen("test.txt","r");
while(fgets(line, sizeof(line), sfile) != NULL)
{
	float j=0; int n=0;
	for (int s=0;s<strlen(line);s++)
	{
		int val = line[s];
		if(val == 58)
		{
			te[test_count].events[n] = j;
			strcpy(te[test_count].line, line);
			n=n+1;
			j=0;
			te_count = n;
		}
		else
		{
			float h = line[s] - '0';
			j = concat(j,h);
		}

	}
n=0;
test_count = test_count+1;
}
fclose(sfile);

/*
cout << "The total events in test files are : " << test_count << endl;
cout << "The total number of events in each array : " << te_count << endl;

for(int k=0;k<test_count;k++)
{
	for(int i=0;i<te_count;i++)
	{
		cout <<  te[k].events[i] << "\t";
	}
	cout << endl;
}
*/

//mean and standard deviation calculation
for(int h=0;h<no;h++)
{
	strcpy(cl[h].event, el[h].event);
	cl[h].weight = el[h].weight;
	//cout << "Event: " << el[h].event << "\t weight: " << el[h].weight << endl;
	float average = 0;
	for(int s=0;s<base_count;s++)
	{
		average = ba[s].events[h] + average;
	}
	cl[h].avg = average/base_count;

	//STANDARD DEVIATION
	float sum = 0.0, mean, sd = 0.0;
	for(int i=0;i<base_count; i++)
	{
		sum += ba[i].events[h];
	}
	//cout << "sum: " << sum << endl; 
	mean = sum/base_count;
	//cout << "mean: " << mean << endl; 
	for(int g=0;g<base_count;g++)
		sd += pow(ba[g].events[h]-mean,2);

	float final_sd = sqrt(sd/base_count);
	//cout << "sd: " << final_sd << endl;
	cl[h].sd = final_sd;
//cout << endl;
//getch();
}

//table printing;
cout << setw(15) << "Event" << setw(15) << "Mean" << setw(30) << "Standard Deviation" << setw(15) << "Weight" << endl;
for(int f=0;f<no;f++)
{
	cout << setw(15) << cl[f].event << setw(15) << cl[f].avg  << setw(30) << cl[f].sd  << setw(15) << cl[f].weight << endl;
}

cout << endl;

//Threshold Calculation
float th= 0.0;
for(int d=0;d<no;d++)
{
	//cout << "th: " << th << endl;
	th = th + el[d].weight;
}
th = 2 *th;
cout << "Threshold:\t" << th << endl;

cout << endl;
//Question 3rd answer
for(int c=0;c<test_count;c++)
{
	float sum = 0.0;
	for(int g=0;g<no;g++)
	{
		float diff;
		diff = fabs(((te[c].events[g] - cl[g].avg)/cl[g].sd));
		float ans = diff * cl[g].weight;
		sum = sum + ans;
	}

	if(sum < th)
	{
		cout << te[c].line << "\t Distance: " << sum << "\t Alarm: No " << endl;
	}
	else
	{
		cout << te[c].line << "\t Distance: " << sum << "\t Alarm: Yes " << endl;
	}
	sum = 0.0;
}

//getch();
}