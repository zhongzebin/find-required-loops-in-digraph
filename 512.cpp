#include <bits/stdc++.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include <sys/stat.h>
using namespace std;

vector<int> EW[500000][4];
vector<int> EWinv[500000][4];
vector<unsigned int> EW_money[500000][4];
vector<unsigned int> EWinv_money[500000][4];
vector<int> EWt[500000];
vector<int> EWinvt[500000];
vector<unsigned int> EW_moneyt[500000];
vector<unsigned int> EWinv_moneyt[500000];
string idsComma[500000];
string idsLF[500000];
int ans3[4][3 * 20000000];
int ans4[4][4 * 20000000];
int ans5[4][5 * 20000000];
int ans6[4][6 * 20000000];
int ans7[4][7 * 20000000];
int r[5][4]={0};
unsigned int inputs[2000000 * 3];
unsigned int tempG[2000000*2];
int c = 0;
unsigned int pre_money[4][7];
bool marked[4][500000] = { false };
int pure[4][500000];
unsigned int pure_money[4][500000];

void GetDataOpt(string &testFile,unsigned int* inputs,unsigned int* tempG,int* c) {
	int fd = open(testFile.c_str(), O_RDONLY);
	int len = lseek(fd, 0, SEEK_END);
	char* p = (char*)mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
	int i = 0;
	unsigned int u;
	char temp[10];
	char tempCount=0;
	int c_i=len/sizeof(char);
	int temp_max=0;
	while (i<c_i)
	{
		for(int ii=0;ii<2;ii++)
		{
			while(p[i] != ',')
			{
				temp[tempCount]=p[i]-'0';
				tempCount++;
				i++;
			}
			unsigned int multi = 1;
			u = 0;
			for (int k = tempCount - 1; k >= 0; k--)
			{
				u += temp[k] * multi;
				multi *= 10;
			}
			tempCount=0;
			inputs[*c] = u;
			tempG[*c/3*2+ii] = u;
			if(temp_max<*c/3*2+ii) temp_max=*c/3*2+ii;
			//(*tempG).emplace_back(u);
			(*c)++;
			i++;
		}
		while(p[i]!='\n')
		{
			temp[tempCount]=p[i]-'0';
			tempCount++;
			i++;
		}
		unsigned int multi = 1;
		u = 0;
		int k;
		k=tempCount-2;
		for (; k >= 0; k--)
		{
			u += temp[k] * multi;
			multi *= 10;
		}
		tempCount=0;
		inputs[*c] = u;
		(*c)++;
		i++;
	}
	//cout<<"max:"<<temp_max<<endl;
	//cout<<"c:"<<*c<<endl;
	munmap(p, len);
	close(fd);
}

string uto_string(unsigned int a)
{
	string ans="";
	if(a==0) return "0";
	char temp[10];
	int i=0;
	while(a)
	{
		temp[i]=a%10;
		a=a/10;
		i++;
	}
	for(int k=i-1;k>=0;k--)
		ans+=temp[k]+'0';
	return ans;
}

struct graph_thread{
	vector<int> (*EW)[4];
	vector<unsigned int> (*EW_money)[4];
	vector<int> (*EWinv)[4];
	vector<unsigned int> (*EWinv_money)[4];
	unordered_map<unsigned int,int> hash_int_id;
	char threadNo;
	int end;
	int begin;
	unsigned int* inputs;
};

void *make_graph(void *arg)
{
	struct graph_thread *a;
	a=(struct graph_thread*) arg;
	char threadNo=a->threadNo;
	for (int i = a->begin; i < a->end; i += 3) {
		int u = a->hash_int_id[a->inputs[i]], v = a->hash_int_id[a->inputs[i + 1]];
		unsigned int m=a->inputs[i + 2];
		EW[u][threadNo].emplace_back(v);
		EWinv[v][threadNo].emplace_back(u);//反向图
		EW_money[u][threadNo].emplace_back(m);
		EWinv_money[v][threadNo].emplace_back(m);
		//if(idsComma[u]=="587," && idsComma[v]=="293,") cout<<idsComma[EWinv[v][0]]<<EWinv_money[v][0]<<endl;
	}
}

void Graph(vector<int> (*EW)[4],vector<unsigned int> (*EW_money)[4],vector<int> (*EWinv)[4],vector<unsigned int> (*EWinv_money)[4],string* idsComma,string* idsLF,int* node_count,int c,unsigned int* inputs,unsigned int* tempG) {
	sort(tempG,tempG+c/3*2);
	bool flag=true;
	unsigned int tG = 0;
	unordered_map<unsigned int,int> hash_int_id;
	for (int i = 0; i<c/3*2; i++)
	{
		if (tempG[i] != tG || flag)
		{
			idsComma[*node_count] = uto_string(tempG[i]) + ',';
			idsLF[*node_count] = uto_string(tempG[i]) + '\n';
			hash_int_id[tempG[i]] = (*node_count)++;
			tG = tempG[i];
			flag=false;
		}
	}
	struct graph_thread a0,a1,a2,a3;
	a0.EW=EW;a0.EW_money=EW_money;a0.EWinv=EWinv;a0.EWinv_money=EWinv_money;a0.hash_int_id=hash_int_id;a0.inputs=inputs;a0.threadNo=0;a0.begin=0;a0.end=c/4/3*3;
	a1.EW=EW;a1.EW_money=EW_money;a1.EWinv=EWinv;a1.EWinv_money=EWinv_money;a1.hash_int_id=hash_int_id;a1.inputs=inputs;a1.threadNo=1;a1.begin=c/4/3*3;a1.end=c/2/3*3;
	a2.EW=EW;a2.EW_money=EW_money;a2.EWinv=EWinv;a2.EWinv_money=EWinv_money;a2.hash_int_id=hash_int_id;a2.inputs=inputs;a2.threadNo=2;a2.begin=c/2/3*3;a2.end=c/4*3;
	a3.EW=EW;a3.EW_money=EW_money;a3.EWinv=EWinv;a3.EWinv_money=EWinv_money;a3.hash_int_id=hash_int_id;a3.inputs=inputs;a3.threadNo=3;a3.begin=c/4*3;a3.end=c;
	pthread_t first,second,third,forth;
	pthread_create(&first,NULL,make_graph,(void*)&a0);
	pthread_create(&second,NULL,make_graph,(void*)&a1);
	pthread_create(&third,NULL,make_graph,(void*)&a2);
	pthread_create(&forth,NULL,make_graph,(void*)&a3);
	pthread_join(first,NULL);
	pthread_join(second,NULL);
	pthread_join(third,NULL);
	pthread_join(forth,NULL);
}

void quickSort2(int left, int right,vector<int>* i,vector<unsigned int>* mi)
{
	if (left < right)
	{
		int pivot = (*i)[left];
		unsigned int pivot_m=(*mi)[left];
		int low = left, high = right;
		while (low < high)
		{
			while (low<high && (*i)[high]>=pivot)
				high--;
			(*i)[low] = (*i)[high];
			(*mi)[low]=(*mi)[high];

			while (low<high && pivot>=(*i)[low])
				low++;
			(*i)[high] = (*i)[low];
			(*mi)[high]=(*mi)[low];
		}
		(*i)[low] = pivot;
		(*mi)[low]=pivot_m;

		quickSort2(left, low - 1,i,mi);
		quickSort2(low + 1, right,i,mi);
	}
}

struct topothread{
	int begin;
	int end;
	vector<int> (*EW)[4];
	vector<int> (*EWinv)[4];
	vector<unsigned int> (*EW_money)[4];
	vector<unsigned int> (*EWinv_money)[4];
	vector<int>* EWt;
	vector<int>* EWinvt;
	vector<unsigned int>* EW_moneyt;
	vector<unsigned int>* EWinv_moneyt;
};

void *topoSort1(void *arg) {
	struct topothread *a;
	a=(struct topothread*) arg;
	for (int i = a->begin; i < a->end; i++) {
		a->EWinvt[i].insert(a->EWinvt[i].end(),a->EWinv[i][0].begin(),a->EWinv[i][0].end());
		a->EWinvt[i].insert(a->EWinvt[i].end(),a->EWinv[i][1].begin(),a->EWinv[i][1].end());
		a->EWinvt[i].insert(a->EWinvt[i].end(),a->EWinv[i][2].begin(),a->EWinv[i][2].end());
		a->EWinvt[i].insert(a->EWinvt[i].end(),a->EWinv[i][3].begin(),a->EWinv[i][3].end());
		a->EWt[i].insert(a->EWt[i].end(),a->EW[i][0].begin(),a->EW[i][0].end());
		a->EWt[i].insert(a->EWt[i].end(),a->EW[i][1].begin(),a->EW[i][1].end());
		a->EWt[i].insert(a->EWt[i].end(),a->EW[i][2].begin(),a->EW[i][2].end());
		a->EWt[i].insert(a->EWt[i].end(),a->EW[i][3].begin(),a->EW[i][3].end());
		a->EWinv_moneyt[i].insert(a->EWinv_moneyt[i].end(),a->EWinv_money[i][0].begin(),a->EWinv_money[i][0].end());
		a->EWinv_moneyt[i].insert(a->EWinv_moneyt[i].end(),a->EWinv_money[i][1].begin(),a->EWinv_money[i][1].end());
		a->EWinv_moneyt[i].insert(a->EWinv_moneyt[i].end(),a->EWinv_money[i][2].begin(),a->EWinv_money[i][2].end());
		a->EWinv_moneyt[i].insert(a->EWinv_moneyt[i].end(),a->EWinv_money[i][3].begin(),a->EWinv_money[i][3].end());
		a->EW_moneyt[i].insert(a->EW_moneyt[i].end(),a->EW_money[i][0].begin(),a->EW_money[i][0].end());
		a->EW_moneyt[i].insert(a->EW_moneyt[i].end(),a->EW_money[i][1].begin(),a->EW_money[i][1].end());
		a->EW_moneyt[i].insert(a->EW_moneyt[i].end(),a->EW_money[i][2].begin(),a->EW_money[i][2].end());
		a->EW_moneyt[i].insert(a->EW_moneyt[i].end(),a->EW_money[i][3].begin(),a->EW_money[i][3].end());
		int size0=EWinvt[i].size(),size1=EWt[i].size();
		if (size0 != 0) {
			quickSort2(0, size0-1,&(a->EWinvt[i]),&(a->EWinv_moneyt[i]));
		}
		if (size1 != 0) {
			quickSort2(0, size1-1,&(a->EWt[i]),&(a->EW_moneyt[i]));
		}
	}
}

struct solvethread{
	int begin;
	int end;
	vector<int>* EWinv;
	vector<unsigned int>* EWinv_money;
	vector<int>* EW;
	vector<unsigned int>* EW_money;
	int* res_count;
	unsigned int (*pre_money)[7];
	unsigned int (*pure_money)[500000];
	int (*pure)[500000];
	bool (*marked)[500000];
	int (*ans3)[3 * 20000000];
	int (*ans4)[4 * 20000000];
	int (*ans5)[5 * 20000000];
	int (*ans6)[6 * 20000000];
	int (*ans7)[7 * 20000000];
	int (*r)[4];
	char threadNo;
};

void dfs(int k,int* loop,bool* marked,int* thread_pure,unsigned int* thread_money,int loopCount,vector<int>* EW,vector<unsigned int>* EW_money,int p_o,int* res_count,unsigned int* thread_pre,int (*ans3)[3 * 20000000],int (*ans4)[4 * 20000000],int (*ans5)[5 * 20000000],int (*ans6)[6 * 20000000],int (*ans7)[7 * 20000000],char threadNo,int (*r)[4]) {
	vector<int> ewCur = EW[k];
	for (int it=0; it < ewCur.size(); it++) {
		int v = ewCur[it];
		if (v < p_o) continue;
		//if(idsComma[v]=="25699," && pure[v]==-2) {cout<<pure_money[v]<<endl;cout<<EW_money[k][it]<<endl;}
		double t1=thread_money[v];
		double t2=EW_money[k][it];
		double c1=t1/t2;
		double c2=thread_pre[0]/t1;
		double c3=t2/thread_pre[loopCount-2];
		if (thread_pure[v] == -2 && !marked[v] && c1>=0.2 && c1<=3 && c2>=0.2 && c2<=3 && c3>=0.2 && c3<=3) {//探索最后一个节点v
			loop[loopCount]=v;
			loopCount++;
			if(loopCount>2){
				int loop_index=loopCount-3;
				for (int i = 0; i < loopCount; i++) {
					//cout<<idsComma[loop[i]];
					switch(loop_index)
					{
						case 0:
							ans3[threadNo][r[0][threadNo]]=loop[i];
							r[0][threadNo]++;
							break;
						case 1:
							ans4[threadNo][r[1][threadNo]]=loop[i];
							r[1][threadNo]++;
							break;
						case 2:
							ans5[threadNo][r[2][threadNo]]=loop[i];
							r[2][threadNo]++;
							break;
						case 3:
							ans6[threadNo][r[3][threadNo]]=loop[i];
							r[3][threadNo]++;
							break;
						case 4:
							ans7[threadNo][r[4][threadNo]]=loop[i];
							r[4][threadNo]++;
							break;
					}
				}
				//cout<<endl;
				(*res_count)++;
			}
			loopCount--;
		}
		if (marked[v] || (thread_pure[v] != p_o && thread_pure[v]!= -2)) continue;
		if (loopCount == 6 || v == p_o) continue;
		if(loopCount==1) thread_pre[0]=t2;
		else if(c3<0.2 || c3>3) continue;
		else thread_pre[loopCount-1]=t2;
		marked[v] = true;
		loop[loopCount]=v;
		dfs(v,loop,marked,thread_pure,thread_money,loopCount+1,EW,EW_money,p_o,res_count,thread_pre,ans3,ans4,ans5,ans6,ans7,threadNo,r);
		marked[v] = false;
	}
}

// 3邻域剪枝思路：如果将图看作是无向图，一个点数为7的环中，距离起点最远的点距离不超过3
void dfspure(int k, int range,int p_o,vector<int>* EW,int* pure,bool* marked) {
	vector<int> ewCur = EW[k];
	for (int it = 0; it < ewCur.size(); it++) {
		int temp=ewCur[it];
		if (temp < p_o || marked[temp]) continue;
		pure[temp] = p_o;
		if (range == 3) continue;
		marked[temp] = true;
		dfspure(temp, range + 1,p_o,EW,pure,marked);
		marked[temp] = false;
	}
}
void dfspure1(int k, int range,int p_o,vector<int>* EWinv,int* pure,bool* marked) {
	vector<int> ewCur = EWinv[k];
	for (int it = 0; it < ewCur.size(); it++) {
		int temp=ewCur[it];
		if (temp < p_o || marked[temp]) continue;
		pure[temp] = p_o;
		if (range == 3) continue;
		marked[temp] = true;
		dfspure1(temp, range + 1,p_o,EWinv,pure,marked);
		marked[temp] = false;
	}
}

void *solve(void *arg) {
	struct solvethread *a;
	a=(struct solvethread*) arg;
	int loop[7];
	char loopCount=0;
	char threadNo=a->threadNo;
	unsigned int* thread_money=a->pure_money[threadNo];
	unsigned int* thread_pre=a->pre_money[threadNo];
	int* thread_pure=a->pure[threadNo];
	bool* thread_marked=a->marked[threadNo];
	for (int i = a->begin; i < a->end; i++) {
		//if(i%100==0)
		//cout<<i<<"/"<<node_count<<endl;
		vector<int> temp=a->EWinv[i];
		vector<unsigned int> temp_money=a->EWinv_money[i];
		dfspure(i, 1,i,a->EW,thread_pure,thread_marked);
		dfspure1(i, 1,i,a->EWinv,thread_pure,thread_marked);
		for (int j = 0; j < temp.size(); j++) {
			int temp_index=temp[j];
			thread_pure[temp_index] = -2;
			thread_money[temp_index]=temp_money[j];
			//if(idsComma[i]=="293,") cout<<j<<' '<<temp_money[j]<<' '<<idsLF[temp[j]];
		}
		loop[loopCount]=i;
		loopCount++;
		dfs(i,loop,thread_marked,thread_pure,thread_money,loopCount,a->EW,a->EW_money,i,&(a->res_count[threadNo]),thread_pre,a->ans3,a->ans4,a->ans5,a->ans6,a->ans7,threadNo,a->r);
		loopCount--;
		for (int j = 0; j < temp.size(); j++) {
			thread_pure[temp[j]] = i;
		}
	}
}

void save(string &outputFile,string* idsComma,string* idsLF,int* res_count,int (*ans3)[3 * 20000000],int (*ans4)[4 * 20000000],int (*ans5)[5 * 20000000],int (*ans6)[6 * 20000000],int (*ans7)[7 * 20000000],int (*r)[4]) {
	int total=res_count[0]+res_count[1]+res_count[2]+res_count[3];
	//printf("Total Loops %d\n", total);
	FILE* fp;
	fp = fopen(outputFile.c_str(), "w");
	char F[10];
	int cnt = 0;
	while (total > 0)
	{
		F[cnt++] = total % 10 + '0';
		total /= 10;
	}
	while (cnt > 0) fwrite(&F[--cnt], sizeof(char), 1, fp);
	char h = '\n', z = '0', d = ',';
	fwrite(&h, sizeof(char), 1, fp);
	for(int i=0;i<4;i++)
		for (int j = 0; j < r[0][i]/3; j++) {
			for (int k = 0; k < 2; k++) {
				auto idres = idsComma[ans3[i][j * 3 + k]];
				fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
			}
			int ii=ans3[i][j * 3 + 2];
			auto idres = idsLF[ii];
			fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
		}
	for(int i=0;i<4;i++)
		for (int j = 0; j < r[1][i]/4; j++) {
			for (int k = 0; k < 3; k++) {
				auto idres = idsComma[ans4[i][j * 4 + k]];
				fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
			}
			int ii=ans4[i][j * 4 + 3];
			auto idres = idsLF[ii];
			fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
		}
	for(int i=0;i<4;i++)
		for (int j = 0; j < r[2][i]/5; j++) {
			for (int k = 0; k < 4; k++) {
				auto idres = idsComma[ans5[i][j * 5 + k]];
				fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
			}
			int ii=ans5[i][j * 5 + 4];
			auto idres = idsLF[ii];
			fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
		}
	for(int i=0;i<4;i++)
		for (int j = 0; j < r[3][i]/6; j++) {
			for (int k = 0; k < 5; k++) {
				auto idres = idsComma[ans6[i][j * 6 + k]];
				fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
			}
			int ii=ans6[i][j * 6 + 5];
			auto idres = idsLF[ii];
			fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
		}
	for(int i=0;i<4;i++)
		for (int j = 0; j < r[4][i]/7; j++) {
			for (int k = 0; k < 6; k++) {
				auto idres = idsComma[ans7[i][j * 7 + k]];
				fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
			}
			int ii=ans7[i][j * 7 + 6];
			auto idres = idsLF[ii];
			fwrite(idres.c_str(), idres.size(), sizeof(char), fp);
		}
}

int main()
{
	int node_count=0;
	string testFile = "/data/test_data.txt";
	string outputFile = "/projects/student/result.txt";
	int res_count[4]={0};
	//auto t = clock(), org = clock();
	//cout<<"begin"<<endl;
	GetDataOpt(testFile,inputs,tempG,&c);
	//cout << "parseInput time:" << double(clock() - t) / CLOCKS_PER_SEC << endl;
	//t = clock();
	Graph(EW,EW_money,EWinv,EWinv_money,idsComma,idsLF,&node_count,c,inputs,tempG);
	//cout<<"graph time:" << double(clock() - t) / CLOCKS_PER_SEC << endl;
	//cout<<"node:"<<node_count<<endl;
	//t=clock();
	struct topothread a0,a1,a2,a3;
	a0.begin=0;a0.end=node_count/4;a0.EW=EW;a0.EWinv=EWinv;a0.EW_money=EW_money;a0.EWinv_money=EWinv_money;a0.EWt=EWt;a0.EWinvt=EWinvt;a0.EW_moneyt=EW_moneyt;a0.EWinv_moneyt=EWinv_moneyt;
	a1.begin=node_count/4;a1.end=node_count/2;a1.EW=EW;a1.EWinv=EWinv;a1.EW_money=EW_money;a1.EWinv_money=EWinv_money;a1.EWt=EWt;a1.EWinvt=EWinvt;a1.EW_moneyt=EW_moneyt;a1.EWinv_moneyt=EWinv_moneyt;
	a2.begin=node_count/2;a2.end=node_count/4*3;a2.EW=EW;a2.EWinv=EWinv;a2.EW_money=EW_money;a2.EWinv_money=EWinv_money;a2.EWt=EWt;a2.EWinvt=EWinvt;a2.EW_moneyt=EW_moneyt;a2.EWinv_moneyt=EWinv_moneyt;
	a3.begin=node_count/4*3;a3.end=node_count;a3.EW=EW;a3.EWinv=EWinv;a3.EW_money=EW_money;a3.EWinv_money=EWinv_money;a3.EWt=EWt;a3.EWinvt=EWinvt;a3.EW_moneyt=EW_moneyt;a3.EWinv_moneyt=EWinv_moneyt;
	pthread_t first,second,third,forth;
	pthread_create(&first,NULL,topoSort1,(void*)&a0);
	pthread_create(&second,NULL,topoSort1,(void*)&a1);
	pthread_create(&third,NULL,topoSort1,(void*)&a2);
	pthread_create(&forth,NULL,topoSort1,(void*)&a3);
	pthread_join(first,NULL);
	pthread_join(second,NULL);
	pthread_join(third,NULL);
	pthread_join(forth,NULL);
	//cout << "topoSort time:" << double(clock() - t) / CLOCKS_PER_SEC << endl;
	//t = clock();
	struct solvethread s0,s1,s2,s3;
	s0.begin=0;s0.end=node_count/40;s0.threadNo=0;s0.EWinv=EWinvt;s0.EW=EWt;s0.EW_money=EW_moneyt;s0.EWinv_money=EWinv_moneyt;s0.r=r;
	s0.res_count=res_count;s0.pre_money=pre_money;s0.pure_money=pure_money;s0.pure=pure;s0.marked=marked;s0.ans3=ans3;s0.ans4=ans4;s0.ans5=ans5;s0.ans6=ans6;s0.ans7=ans7;
	s1.begin=node_count/40;s1.end=node_count/20;s1.threadNo=1;s1.EWinv=EWinvt;s1.EW=EWt;s1.EW_money=EW_moneyt;s1.EWinv_money=EWinv_moneyt;s1.r=r;
	s1.res_count=res_count;s1.pre_money=pre_money;s1.pure_money=pure_money;s1.pure=pure;s1.marked=marked;s1.ans3=ans3;s1.ans4=ans4;s1.ans5=ans5;s1.ans6=ans6;s1.ans7=ans7;
	s2.begin=node_count/20;s2.end=node_count/10;s2.threadNo=2;s2.EWinv=EWinvt;s2.EW=EWt;s2.EW_money=EW_moneyt;s2.EWinv_money=EWinv_moneyt;s2.r=r;
	s2.res_count=res_count;s2.pre_money=pre_money;s2.pure_money=pure_money;s2.pure=pure;s2.marked=marked;s2.ans3=ans3;s2.ans4=ans4;s2.ans5=ans5;s2.ans6=ans6;s2.ans7=ans7;
	s3.begin=node_count/10;s3.end=node_count;s3.threadNo=3;s3.EWinv=EWinvt;s3.EW=EWt;s3.EW_money=EW_moneyt;s3.EWinv_money=EWinv_moneyt;s3.r=r;
	s3.res_count=res_count;s3.pre_money=pre_money;s3.pure_money=pure_money;s3.pure=pure;s3.marked=marked;s3.ans3=ans3;s3.ans4=ans4;s3.ans5=ans5;s3.ans6=ans6;s3.ans7=ans7;
	pthread_create(&first,NULL,solve,(void*)&s0);
	pthread_create(&second,NULL,solve,(void*)&s1);
	pthread_create(&third,NULL,solve,(void*)&s2);
	pthread_create(&forth,NULL,solve,(void*)&s3);
	pthread_join(first,NULL);
	pthread_join(second,NULL);
	pthread_join(third,NULL);
	pthread_join(forth,NULL);
	//cout << "solve time:" << double(clock() - t) / CLOCKS_PER_SEC << endl;
	//t=clock();
	save(outputFile,idsComma,idsLF,res_count,ans3,ans4,ans5,ans6,ans7,r);
	//cout << "save time:" << double(clock() - t) / CLOCKS_PER_SEC << endl;
	//cout << "total time:" << double(clock() - org) / CLOCKS_PER_SEC << endl;

	return 0;
}

