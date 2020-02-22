#include<bits/stdc++.h>
#include<iostream>
using namespace std;

#define N 1050
#define k1 1050


int k;
int T[N][k1+1]; //T[v][t]


int root;
int num_nodes;
int num_edges;
int num_leaves;
int tree[N];
int postTree[N];
int post_order_tree[N];
int invPostOrder[N];
int leaves[N];
int m[N]; 
int m_dash[N];
int node_weight[N];
unordered_map<int,int> edge_weight;

int D[N];
int tu1[N];

int wt_below[N];

set<int> nodes;
set<int> arr[N];
set<int> postArr[N];
int leaf_map[N];


int create_distance(int i){
	D[i]=i==root?0:create_distance(tree[i])+edge_weight[(i*num_nodes)+tree[i]];
	return D[i];
}
int distance(int u,int v){
	return (D[u]-D[v]);
}

int weight_below(int root){
	wt_below[root]=node_weight[root];
	for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
		wt_below[root]+=weight_below((*itr));
	}
	return wt_below[root];
}

int compute_tu1(int root){
	tu1[root]=0;
	for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
		tu1[root]+=(wt_below[(*itr)])*edge_weight[((*itr)*num_nodes)+root]+compute_tu1((*itr));
	}
	return tu1[root];
}
int prev_leaf(int leaf){
	return leaf_map[leaf]>0?leaves[leaf_map[leaf]-1]:-1;
}
void create_leaves(int root, int &index){
	if(arr[root].empty()){
		leaves[index]=root;
		leaf_map[root]=index;
		index++;
	}
	else{
		for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
			create_leaves((*itr), index);
		}
	}
	return;
}
void create_mvd(int root){
	m_dash[root]= prev_leaf(root);
	for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
		create_mvd((*itr));
	}
	return;
}
int create_mv(int root){
	int temp=root;int aa=1;m[root]=root;
	for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
		temp=create_mv((*itr));
		if(aa==1) m[root]=temp;
		aa++;
	}
	return m[root];
}

void create_invpostorder(){
	for(int i=0;i<num_nodes;i++){
		invPostOrder[post_order_tree[i]]=i;
	}
	return;
}

void create_postorder(int root, int &index){
	for(auto itr = postArr[root].begin();itr!=postArr[root].end();itr++){
		create_postorder((*itr),index);
	}
	post_order_tree[index]=root;index++;
	return;
}

void initialize(){
	num_leaves=0;
	for(int i=0;i<N;i++){
		tree[i]=-1;
		postTree[i]=-1;
	}
	for(int i=0;i<N;i++){
		arr[i].clear();
		postArr[i].clear();
	}
	num_nodes=0;
	num_edges=0;
	ifstream myFile;
	string line;
	int temp;
	int temp2;
	string wst;
	myFile.open("trace.tree");
	while(getline(myFile,line)){
		stringstream ss(line);
		ss>>temp;
		nodes.insert(temp);
		ss>>wst;
		while(ss>>temp2){
			nodes.insert(temp2);
			postArr[temp].insert(temp2);
			postTree[temp2]=temp;
			num_edges++;
		}
	}
	myFile.close();

	for(int i=0;i<num_nodes;i++){
		if(postTree[i]==-1){
			root=i;
			break;
		}
	}


	int index=0;
	num_nodes=nodes.size();
	create_postorder(root,index);
	create_invpostorder();

	for(int i=0;i<num_nodes;i++){
		for(auto itr=postArr[post_order_tree[i]].begin();itr!=postArr[post_order_tree[i]].end();itr++){
			arr[i].insert(invPostOrder[(*itr)]);
		}
	}

	for(int i=0;i<num_nodes;i++){
		tree[i]=invPostOrder[postTree[post_order_tree[i]]];
	}


	myFile.open("trace.cost");
	while(getline(myFile,line)){
		stringstream ss(line);
		ss>>temp;
		ss>>wst;
		ss>>node_weight[invPostOrder[temp]];
	}
	myFile.close();

	root=invPostOrder[root];

	for(int i=0;i<num_nodes;i++){
		for(auto itr=arr[i].begin();itr!=arr[i].end();itr++){
			edge_weight[((*itr)*num_nodes)+i]=1;
		}
	}

	cin>>k;


	cout<<"The post tree is:\n";
	// for(int i=0;i<num_nodes;i++){
	// 	cout<<postTree[i]<<" ";
	// }
	// cout<<endl;
	
	cout<<"The tree is:\n";
	// for(int i=0;i<num_nodes;i++){
	// 	cout<<tree[i]<<" ";
	// }
	// cout<<endl;

	cout<<"The root in postOrdertree is : "<<root<<endl<<endl;

	create_mv(root);
	cout<<"The mv is: \n";
	// for(int i=0;i<num_nodes;i++){
	// 	cout<<m[i]<<" ";
	// }
	// cout<<endl;

	index=0;
	create_leaves(root,num_leaves);

	cout<<"The leaves are: \n";
	// for(int i=0;i<num_leaves;i++){
	// 	cout<<leaves[i]<<" ";
	// }
	// cout<<endl;

	for(int i=0;i<num_leaves;i++){
		create_distance(leaves[i]);
	}

	create_mvd(root);

	cout<<"The mv_dash is: \n";
	// for(int i=0;i<num_nodes;i++){
	// 	cout<<m_dash[i]<<" ";
	// }
	// cout<<endl;


	index= weight_below(root);
	index = compute_tu1(root);

	cout<<"The wt_below is: \n";
	// for(int i=0;i<num_nodes;i++){
	// 	cout<<wt_below[i]<<" ";
	// }
	// cout<<endl;


	cout<<"The tu1 is: \n";
	// for(int i=0;i<num_nodes;i++){
	// 	cout<<tu1[i]<<" ";
	// }
	// cout<<endl;

	distance(root,root);
	cout<<"The distance is: \n";
	// for(int i=0;i<num_nodes;i++){
	// 	cout<<D[i]<<" ";
	// }
	// cout<<endl;

	return;
}
void phase_one(){
	for(int u=0;u<num_nodes;u++){
		int* T_u = new int[num_nodes]; //t_u[v]0
		int* R_u= new int[num_nodes]; //R_u[v]1
		int** L_u=new int*[num_nodes]; //L_u_v_t;
		// T[v][1]

		for(int i=0;i<num_nodes;i++){
			L_u[i]=new int[k+1];
		}

		int* tmp_arr = new int[u-m[u]+2];
		tmp_arr[0]=0;
		for(int i=0;i<(u-m[u]+1);i++){
			tmp_arr[i+1]=tmp_arr[i]+node_weight[i+m[u]]*distance(i+m[u],u);
		}
		for(int i=m[u];i<=u;i++){
			T_u[i]= tmp_arr[i-m[u]+1]  - tmp_arr[m[i]-m[u]];
		}
		for(int i=m[u];i<=u;i++){
			R_u[i]= tmp_arr[u-m[u]+1]  - tmp_arr[i-m[u]+1];
		}
		for(int i=m[u];i<=u;i++){
			L_u[i][0]= tmp_arr[m[i]-m[u]]  - tmp_arr[0];
		}

		for(int i=m[u];i<=u;i++){
			T[i][1]=tu1[i];
		}

		// initialize();
		delete[] tmp_arr;
		

		for(int t=2;t<=k;t++){
			for(int v=m[u]+1;v<u;v++){
				if(v==m[v] && m[v]!=m[u]){
					// L_u_v_t-1 calculation
					
					int a = L_u[m_dash[v]][t-1] + R_u[m_dash[v]] + T_u[m_dash[v]] - R_u[v] - T_u[v]; //Au,v,t = L(u,m'v,t) + R(u,m'v)1 + T(u,m'v,0) − R(u,v,1) − T(u,v,0)
					int b = INT_MAX;
					for(int x=m_dash[v];x<m[v];x++){
						for(int t_dash = 1;t_dash<=t-1  ;t_dash++){
							if(t_dash<=x-m[x]+1 && t-(m[x]-m[u])-1<=t_dash)
								b=min(b,T[x][t_dash] + L_u[x][t-1-t_dash] + R_u[x] - R_u[v] - T_u[v]);
						}
					}
					L_u[v][t-1] = min(a,b);
				}
				////Added by meee
				else{
					if(m[v]==m[u]){
						L_u[v][t-1] = 0;
					}else{
						L_u[v][t-1] = L_u[m[v]][t-1];
					}
				}
			}
			T[u][t]=INT_MAX;
			for(int v=m[u];v<u;v++){
				for(int t_dash = 1;t_dash<t  ;t_dash++){
					if(t_dash<=v-m[v]+1 && t-(m[v]-m[u])-1<=t_dash)
						T[u][t]=min(T[u][t],T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v] );
				}
			}
		}
			
			delete[] T_u;
			delete[] R_u;
			for(int i=0;i<num_nodes;i++){
				delete[] L_u[i];
			}
			delete[] L_u;

		
	}
	return;
}


set<int> proxies(int u, int t){
	set<int> res;
	res.insert(u);
	if(t==1) return res;
	int* T_u = new int[num_nodes]; //t_u[v]0
	int* R_u= new int[num_nodes]; //R_u[v]1
	int** L_u=new int*[num_nodes]; //L_u_v_t;
	// T[v][1]

	for(int i=0;i<num_nodes;i++){
		L_u[i]=new int[k+1];
	}

	int* tmp_arr = new int[u-m[u]+2];
	tmp_arr[0]=0;
	for(int i=0;i<(u-m[u]+1);i++){
		tmp_arr[i+1]=tmp_arr[i]+node_weight[i+m[u]]*distance(i+m[u],u);
	}
	for(int i=m[u];i<=u;i++){
		T_u[i]= tmp_arr[i-m[u]+1]  - tmp_arr[m[i]-m[u]];
	}
	for(int i=m[u];i<=u;i++){
		R_u[i]= tmp_arr[u-m[u]+1]  - tmp_arr[i-m[u]+1];
	}
	for(int i=m[u];i<=u;i++){
		L_u[i][0]= tmp_arr[m[i]-m[u]]  - tmp_arr[0];
	}

	for(int i=m[u];i<=u;i++){
		T[i][1]=tu1[i];
	}

	delete[] tmp_arr;
	//initialisation();
	


	int* N_u = new int[k+1];
	int* C_u = new int[k+1];
	int** x_u=new int*[num_nodes];
	int** c_u=new int*[num_nodes];
	for(int i=0;i<num_nodes;i++){
		x_u[i]=new int[k+1];
		c_u[i]=new int[k+1];
	}
	for(int t=2;t<=k;t++){
		for(int v=m[u]+1;v<u;v++){
			if(v==m[v] && m[v]!=m[u]){
				//L_u_v_t-1 calculation
				int b = INT_MAX;
				for(int x=m[u];x<m[v];x++){
					for(int t_dash = 1;t_dash<t ;t_dash++){
						if(b>T[x][t_dash] + L_u[x][t-1-t_dash] + R_u[x] - R_u[v] - T_u[v]  && t_dash<=x-m[x]+1 && t-(m[x]-m[u])-1<=t_dash ){
							x_u[v][t-1] = x;
							c_u[v][t-1] = t_dash;
							b=T[x][t_dash] + L_u[x][t-1-t_dash] + R_u[x] - R_u[v] - T_u[v];
						}
					}
				}
				if(b==INT_MAX) b=0;
				L_u[v][t-1] = b;
			}
			/////added by me
			else{
				if(m[v]==m[u]){
					L_u[v][t-1] = 0;
					x_u[v][t-1] = m[u];
					c_u[v][t-1] = 1;
				}else{
					L_u[v][t-1] = L_u[m[v]][t-1];
					x_u[v][t-1] = x_u[m[v]][t-1];
					c_u[v][t-1] = c_u[m[v]][t-1];
				}
			}
		}
	}
	int lol = INT_MAX;
	for(int v=m[u];v<u;v++){
		for(int t_dash = 1;t_dash<t ;t_dash++){
			if(lol>=T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v] && t_dash<=v-m[v]+1 && t-(m[v]-m[u])-1<=t_dash){

				N_u[1]=v;
				C_u[1]=t_dash;
				lol=T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v];
			}
		}
	}
	int i=2;
	t=t-C_u[1]-1;

	while(t>0){
		
		C_u[i] = c_u[N_u[i-1]][t];
		N_u[i] = x_u[N_u[i-1]][t];
		t=t-C_u[i];
		i++;

	}
	delete[] T_u;
	delete[] R_u;
	for(int i=0;i<num_nodes;i++){
		delete[] L_u[i];
		delete[] x_u[i];
		delete[] c_u[i];

	}

	delete[] L_u;
	delete[] x_u;
	delete[] c_u;
	for(int temp=1;temp<i;temp++){
		set<int> temp_set = proxies(N_u[i-temp],C_u[i-temp]);
		res.insert(temp_set.begin(),temp_set.end());
	}
	delete[] N_u;
	delete[] C_u;
	return res;
}
int main(){

	clock_t start_time,end_time;
	start_time=clock();
	initialize();
	cout<<"Initialization done\n";
	phase_one();
	cout<<"Phase one completed\n";
	for(int i=0;i<0;i++){

	}
    set<int> result = proxies(root,k);
    cout<<"The proxies in postOrdertree are: \n";
    for(auto i:result){
    	cout<<i<<' ';
    }
    cout<<endl;

    cout<<"The proxies in Actualtree are: \n";
    for(auto i:result){
    	cout<<post_order_tree[i]<<' ';
    }
    cout<<endl;

    end_time=clock();
	cout<<"\nTime taken: "<<(double)(end_time-start_time)<<" milliseconds"<<endl;
	return 0;
}
