#include<iostream>
#include<vector>
#include<queue>
#include<omp.h>
using namespace std;

class Solution{
public:
 	vector<int> dfs(vector<int> adj[],int s,int v){
 		vector<int>  vis(v+1,0);
 		vector<int> ans;
 		dfsREC(adj,s,vis,ans);
 		return ans;
 	}
 	
 	void dfsREC(vector<int> adj[],int node,vector<int> &vis,vector<int> &ans){
 		ans.push_back(node)	;
 		vis[node]=1;
 		for(auto i:adj[node]){
 			if(!vis[i]){
 				dfsREC(adj,i,vis,ans);
 			}
 		}
 	}
 	
 	vector<int> BFS(int v,vector<int> adj[],int s){
		vector<int> ans;
		vector<int> vis(v+1,0);
		queue<int> q;
		q.push(s);
		vis[s]=1;
		while(!q.empty()){
			int node=q.front();
			q.pop();
			ans.push_back(node);
			for(auto i:adj[node]){
				if(!vis[i]){
					q.push(i);
					vis[i]=1;
				}
			}
		}
		return ans;
	}
 	
};

void addE(vector<int> adj[],int a,int b){
	adj[a].push_back(b);
	adj[b].push_back(a);
}


int main(){
	int n,e;
	cout<<"\nEnter number of nodes:";cin>>n;
	cout<<"\nEnter number of edges:";cin>>e;
	vector<int> adj[n+1];
	for(int i=0;i<e;i++){
		int a,b;
		cout<<"\nEnter edge(a b):";
		cin>>a>>b;
		addE(adj,a,b);
	}
	
	int s,t,t1,t2;
	cout<<"\nEnter start node:";
	cin>>s;
	cout<<"\nEnter no. of threads to use:";
	cin>>t;
	Solution obj;
	vector<int> df,ans;
	
	double time;
	time=omp_get_wtime();
	omp_set_num_threads(t);
	#pragma omp parallel sections
	
	{
	#pragma omp section
		{
			t1=omp_get_thread_num();
			df=obj.dfs(adj,s,n);
		}
		
	#pragma omp section
		{	
			t2=omp_get_thread_num();
			ans=obj.BFS(n,adj,s);
		}
	}
	
	double p_time=omp_get_wtime()-time;
	cout<<"\nDFS traversal:";
	for(auto i:df) cout<<i<<" ";
	cout<<endl;	
	cout<<"\nBFS traversal:";
	for(auto i:ans) cout<<i<<" ";
	cout<<endl;
	
	
	cout<<"\nDFS sequential time:";
	time=omp_get_wtime();
	df=obj.dfs(adj,s,n);
	double dfs_s=omp_get_wtime()-time;
	cout<<dfs_s<<"secs"<<endl;
	
	cout<<"\nBFS sequential time:";
	time=omp_get_wtime();
	ans=obj.BFS(n,adj,s);
	double bfs_s=omp_get_wtime()-time;
	cout<<bfs_s<<"secs"<<endl;
	
	cout<<"\nDFS parallel time:";
	time=omp_get_wtime();
	df=obj.dfs(adj,s,n);
	double dfs_p=omp_get_wtime()-time;
	cout<<dfs_p<<"secs"<<endl;
	
	cout<<"\nBFS parallel time:";
	time=omp_get_wtime();
	ans=obj.BFS(n,adj,s);
	double bfs_p=omp_get_wtime()-time;
	cout<<bfs_p<<"secs"<<endl;
	
	//cout<<"\nThread 1:"<<t1;
	//cout<<"\nThread 2:"<<t2;
	
	
	
	
}
