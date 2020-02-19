#include<bits/stdc++.h>
#include<iostream>
using namespace std;

#define N 100
#define k1 100


int k;
int T[N][k1+1]; //T[v][t]


int root;
int num_nodes;
int num_edges;
int num_leaves;
int tree[N];
int post_order_tree[N];
int leaves[N];
int m[N]; 
int m_dash[N];
int node_weight[N];
int edge_weight[N][N];

int D[N][N];
int tu1[N];

int tuv[N][N];


int wt_below[N];

set<int> arr[N];
int leaf_map[N];

int ruv[N][N];
int luv[N][N];


void fill_luv(int root){
	int mroot= m[root];int temp = 0;
	for(int j=mroot;j<=root;j++){
		luv[root][j]= leaf_map[j]>0?temp:luv[root][m[j]];
		temp+=D[j][root]*node_weight[j];
	}
	if(!arr[root].empty()){
		for(auto itr= arr[root].begin();itr!=arr[root].end();itr++){
			fill_luv((*itr));
		}
	}
	return;
}


void fill_ruv(int root){
	int mroot = m[root];int temp=0;
	for(int j=root;j>=mroot;j--){
		ruv[root][j]=temp;
		temp+=D[j][root]*node_weight[j];
	}
	if(!arr[root].empty()){
		for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
			fill_ruv((*itr));
		}
	}
	return;
}
void distance(int root,int gl_root){
	int temp_dist=0;int temp=root;D[temp][temp]=0;
	while(temp!=gl_root){
		temp_dist+=edge_weight[temp][tree[temp]];
		D[root][tree[temp]]=temp_dist;
		temp=tree[temp];
	}
	if(!arr[root].empty()){
		for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
			distance((*itr),gl_root);
		}
	}
	return;
}
void tuv0(int root, int gl_root){
	int pp=tu1[root];int temp=root;int tmp_wt=0;
	while(temp!=gl_root){
		tuv[temp][root]=tmp_wt+pp;
		tmp_wt+=(node_weight[root]+wt_below[root])*edge_weight[temp][tree[temp]];
		temp=tree[temp];
	}
	tuv[gl_root][root]=tmp_wt+pp;
	if(!arr[root].empty()){
		for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
			tuv0((*itr),gl_root);
		}
	}
	return;
}

int weight_below(int root){
	wt_below[root]=0;
	if(!arr[root].empty()){
		int temp=0;
		for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
			temp+=weight_below((*itr));
			temp+=node_weight[(*itr)];
		}
		wt_below[root]=temp;
	}
	return wt_below[root];
}

int compute_tu1(int root){
	tu1[root]=0;int temp=0;
	if(!arr[root].empty()){
		for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
			temp+=(wt_below[(*itr)]+node_weight[(*itr)])*edge_weight[(*itr)][root]+compute_tu1((*itr));
		}
		tu1[root]=temp;
	}
	return tu1[root];
}
int prev_leaf(int leaf){
	int index= leaf_map[leaf];
	int ans=index>0?leaves[index-1]:-1;
	return ans;
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
	if(!arr[root].empty()){
		for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
			create_mvd((*itr));
		}
	}
	return;
}
int create_mv(int root){
	int temp=root;int aa=1;m[root]=root;
	if(!arr[root].empty()){
		for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
			temp=create_mv((*itr));
			if(aa==1){
				m[root]=temp;
			}
			aa++;
		}
	}
	return m[root];
}
void create_postorder(int root, int &index){
	for(auto itr = arr[root].begin();itr!=arr[root].end();itr++){
		create_postorder((*itr),index);
	}
	post_order_tree[index]=root;
	index++;
	return;
}

void initialize(){
	cin>>num_nodes>>num_edges>>k;
	num_leaves=0;
	if(num_nodes>N||num_edges>N*N){
		cerr<<"Tree too large!";
		return;
	}
	for(int i=0;i<num_nodes;i++){
		tree[i]=-1;
	}
	for(int i=0;i<num_nodes;i++){
		cin>>node_weight[i];
	}
	for(int i=0;i<num_nodes;i++){
		arr[i].clear();
	}
	int temp;
	for(int i=0;i<num_nodes;i++){
		edge_weight[i][i]=0;
	}
	for(int i=0;i<num_edges;i++){
		cin>>temp>>tree[temp];
		cin>>edge_weight[temp][tree[temp]];
		arr[tree[temp]].insert(temp);
	}
	for(int i=0;i<num_nodes;i++){
		if(tree[i]==-1){
			root=i;
			break;
		}
	}

	cout<<"The tree is:\n";
	for(int i=0;i<num_nodes;i++){
		cout<<tree[i]<<" ";
	}
	cout<<endl;


	int index=0;
	create_postorder(root,index);

	if(index==num_nodes){
		cout<<"The post order traversal is: \n";
		for(int i=0;i<num_nodes;i++){
			cout<<post_order_tree[i]<<" ";
		}
		cout<<endl;
	}
	else{
		cout<<"Something wrong\n";
	}

	create_mv(root);
	cout<<"The mv is: \n";
	for(int i=0;i<num_nodes;i++){
		cout<<m[i]<<" ";
	}
	cout<<endl;

	index=0;
	create_leaves(root,num_leaves);

	cout<<"The leaves are: \n";
	for(int i=0;i<num_leaves;i++){
		cout<<leaves[i]<<" ";
	}
	cout<<endl;

	create_mvd(root);

	cout<<"The mv_dash is: \n";
	for(int i=0;i<num_nodes;i++){
		cout<<m_dash[i]<<" ";
	}
	cout<<endl;

	index= weight_below(root);
	index = compute_tu1(root);

	cout<<"The wt_below is: \n";
	for(int i=0;i<num_nodes;i++){
		cout<<wt_below[i]<<" ";
	}
	cout<<endl;


	cout<<"The tu1 is: \n";
	for(int i=0;i<num_nodes;i++){
		cout<<tu1[i]<<" ";
	}
	cout<<endl;


	tuv0(root,root);
	cout<<"The tuv0 is: \n";
	for(int j=0;j<num_nodes;j++){
		for(int i=0;i<num_nodes;i++){
			cout<<tuv[j][i]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;


	distance(root,root);
	cout<<"The distance is: \n";
	for(int j=0;j<num_nodes;j++){
		for(int i=0;i<num_nodes;i++){
			cout<<D[j][i]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;


	fill_ruv(root);
	cout<<"The ruv is: \n";
	for(int j=0;j<num_nodes;j++){
		for(int i=0;i<num_nodes;i++){
			cout<<ruv[j][i]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;


	fill_luv(root);
	cout<<"The luv is: \n";
	for(int j=0;j<num_nodes;j++){
		for(int i=0;i<num_nodes;i++){
			cout<<luv[j][i]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	return;
}
void phase_one(){
	for(int u=0;u<num_nodes;u++){
		int T_u[num_nodes]={0}; //t_u[v]0
		int R_u[num_nodes]={0}; //R_u[v]1
		int L_u[num_nodes][k+1]={0}; //L_u_v_t;
		

		for(int i=0;i<num_nodes;i++){
			T[i][1]=tu1[i];
			T_u[i]=tuv[u][i];
			R_u[i]=ruv[u][i];
			L_u[i][0]=luv[u][i];
		}


		// initialize();
		
		


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
				// if(t==2 && v<3) cout<<"test"<<v-m[v]+1<<' '<< t-(m[v]-m[u])-1<<endl;
				for(int t_dash = 1;t_dash<t  ;t_dash++){
					// if(t==2) cout<<"yes"<<' '<< u<<' '<<t_dash<< endl;
					if(t_dash<=v-m[v]+1 && t-(m[v]-m[u])-1<=t_dash)
						T[u][t]=min(T[u][t],T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v] );
				}
			}
		}
		
	}

	cout<<"tuuuuv is:\n";
	for(int t=0;t<=k;t++){
		for(int i=0;i<k;i++){
			cout<<T[t][i]<<' ';
		}
		cout<<endl;
	}
	return;
}


set<int> proxies(int u, int t){
	// cout<<endl<<endl<<u<<' '<<t<<endl;
	set<int> res;
	res.insert(u);
	if(t==1) return res;
	int T_u[num_nodes]={0}; //t_u[v]0
	int R_u[num_nodes]={0}; //R_u[v]1
	int L_u[num_nodes][k+1]={0}; //L_u_v_t;
	// T[v][1]


	for(int i=0;i<num_nodes;i++){
		T[i][1]=tu1[i];
		T_u[i]=tuv[u][i];
		R_u[i]=ruv[u][i];
		L_u[i][0]=luv[u][i];
	}


	//initialisation();
	


	int N_u[k+1];
	int C_u[k+1];
	int x_u[num_nodes][k+1];
	int c_u[num_nodes][k+1];
	cout<<"ti "<<t<<endl;
	for(int t=2;t<=k;t++){
		for(int v=m[u]+1;v<u;v++){
			if(v==m[v] && m[v]!=m[u]){
				//L_u_v_t-1 calculation
				//int a = L_u[m_dash[v]][t-1] + R_u[m_dash[v]] + T_u[m_dash[v]] - R_u[v] - T_u[v]; //Au,v,t = L(u,m'v,t) + R(u,m'v)1 + T(u,m'v,0) − R(u,v,1) − T(u,v,0)
				int b = INT_MAX;
				for(int x=m[u];x<m[v];x++){
					for(int t_dash = 1;t_dash<t ;t_dash++){
						cout<<"check "<<x <<' '<<v<<' '<<u<<' '<< t-(m[x]-m[u])-1<<endl;
						if(b>T[x][t_dash] + L_u[x][t-1-t_dash] + R_u[x] - R_u[v] - T_u[v]  && t_dash<=x-m[x]+1 && t-(m[x]-m[u])-1<=t_dash ){
							// cout<<"check "<<T[x][t_dash] <<' '<< L_u[x][t-1-t_dash] <<' '<< R_u[x] << ' '<< - R_u[v] << ' '<<- T_u[v]<<endl;
							// cout<<"this is "<<v<<' '<<t-1<<endl;
							x_u[v][t-1] = x;
							c_u[v][t-1] = t_dash;
							b=T[x][t_dash] + L_u[x][t-1-t_dash] + R_u[x] - R_u[v] - T_u[v];
						}
					}
				}
				cout<<"b "<<b<<endl;
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
			// cout<<"this is "<<L_u[v][t-1]<<endl;
		}
	}
	cout<<"tf "<<t<<endl;
	 // cout<<"hii2";
	int lol = INT_MAX;
	for(int v=m[u];v<u;v++){
		if(v==3) cout<<"coming here "<<v-m[v]+1 <<' '<< t-(m[v]-m[u])-1<<endl;
		for(int t_dash = 1;t_dash<t ;t_dash++){
			cout<<"coming here2 "<<v<<' '<<t_dash<<' '<<lol <<' '<< T[v][t_dash] <<' '<< L_u[m[v]][t-1-t_dash] <<' '<< R_u[v]<<endl;
			if(lol>=T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v] && t_dash<=v-m[v]+1 && t-(m[v]-m[u])-1<=t_dash){

				cout<<"set\n\n\n";
				N_u[1]=v;
				cout<<"highest num pro "<<v<<endl;
				C_u[1]=t_dash;
				lol=T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v];
			}
		}
	}
	// cout<<"hii3 "<<C_u[1]<<endl;
	int i=2;
	t=t-C_u[1]-1;

	cout<<"xu\n";
	
	for(int j=0;j<num_nodes;j++){
		for(int t=1;t<k;t++){
			cout<<x_u[j][t]<<' ';
		}
		cout<<endl;
	}

	
	cout<<"i "<<i<<endl;

	while(t>0){
		
		C_u[i] = c_u[N_u[i-1]][t];
		N_u[i] = x_u[N_u[i-1]][t];
		cout<<t<<' '<<i<<' '<<N_u[i]<<' '<<C_u[i]<<' '<<x_u[N_u[i-1]][t]<<endl;
		t=t-C_u[i];
		i++;

	}

	cout<<u<<endl;

	cout<<"NUUUU\n";
	for(int j=0;j<=i;j++){
		cout<<N_u[j]<<' ';
	}
	cout<<"XUUUU\n";
	for(int j=0;j<=i;j++){
		cout<<C_u[j]<<' ';
	}

	cout<<endl;
	cout<<"hii4"<<endl;
	for(int temp=1;temp<i;temp++){
		cout<<"now "<<u<<' '<<N_u[i-temp]<<' '<<C_u[i-temp]<<' '<<i-temp<<endl;
		set<int> temp_set = proxies(N_u[i-temp],C_u[i-temp]);
		res.insert(temp_set.begin(),temp_set.end());
	}
	return res;
}
int main(){
	initialize();
	phase_one();
	// cout<<"hii1";
    set<int> result = proxies(root,k);
    cout<<"The proxies are: \n";
    for(auto i:result){
    	cout<<i<<' ';
    }
    cout<<endl;
    // cout<<"hii2";
	return 0;
}
