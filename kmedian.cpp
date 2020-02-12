#include <bits/stdc++.h)
using namespace std;
#define num_nodes 100
int k;
int tree[num_nodes];
int post_order_tree[num_nodes];
int m[num_nodes];
int m_dash[num_nodes];
int node_weight[num_nodes];
int edge_wight[num_nodes];
int D[num_nodes][num_nodes]; //D[u][v]
int T[num_nodes][k+1]; //T[v][t]


int d(int u, int v){
	return D[u][v];
}

int w(int u){
	return w[u];
}

set<int> proxies(int u, int t){
	set<int> res;
	res.insert(u);
	if(t==1) return res;
	int T_u[num_nodes]={0}; //t_u[v]0
	int R_u[num_nodes]={0}; //R_u[v]1
	int L_u[num_nodes][k+1]={0}; //L_u_v_t;
	initialisation();
	int N[num_nodes][k+1];
	int C[num_nodes][k+1];
	int x_u[num_nodes][k+1];
	int c_u[num_nodes][k+1];
	for(int t=2;t<=k;t++){
		for(int v=m[u]+1;v<u;v++){
			if(v==m[v]){
				//L_u_v_t-1 calculation
				int a = L_u[m_dash[v]][t-1] + R_u[m_dash[v]] + T_u[m_dash[v]] - R_u[v] - T_u[v]; //Au,v,t = L(u,m'v,t) + R(u,m'v)1 + T(u,m'v,0) − R(u,v,1) − T(u,v,0)
				int b = INT_MAX;
				for(int x=m_dash[v];x<m[v];x++){
					for(int t_dash = 1;t_dash<t && t_dash<=v-m[v]+1 && t-(m[v]-m[u])-1<=t_dash ;t_dash++){
						if(b>T[x][t_dash] + L_u[x][t-1-t_dash] + R_u[x] - R_u[v] - T_u[v]){
							x_u[v][t-1] = x;
							c_u[v][t-1] = t_dash;
						}
						b=min(b,T[x][t_dash] + L_u[x][t-1-t_dash] + R_u[x] - R_u[v] - T_u[v]);
					}
				}
				L_u[v][t-1] = min(a,b);
			}
		}
	}
	
	int lol = INT_MAX;
	for(int v=m[u];v<u;v++){
		for(int t_dash = 1;t_dash<t && t_dash<=v-m[v]+1 && t-(m[v]-m[u])-1<=t_dash ;t_dash++){
			if(lol>T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v]){
				C_u[1]=x;
				N_u=t_dash;
				lol=T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v];
			}
		}
	}
	
	int i=2;
	t=t-C_u[1]-1;
	while(t>0){
		C_u[i] = x_u[N_u[i-1]][t];
		N_u[i] = c_u[N_u[i-1]][t];
		t=t-C_u[i];
		i++;
	}
	for(int temp=1;temp<i;temp++){
		set<int> temp_set = proxies(N_u[i-temp],C_u[i-temp]);
		res.insert(temp_set);
	}
	return res;
}


void initialisation(){
	
}


int main(){
		// L1: Order the nodes of the tree; ∀v compute mv and m0
// v
// , as defined in Section 2.1.
// L2: For u = 1 to n do
// L3: Initialization: For all v ∈ Tu, compute kTvk1, kTu,vk0, kRu,vk1 and kLu,vk0.
// L4: For t = 2 to k do
// L5: For v = mu + 1 to u − 1 do
// L6: If v = mv compute kLu,vkt−1 using (3)
// L7: Compute kTukt using (1).
// L8: Release the memory used, except for kTvkt
// , v ∈ [1..u],t ∈ [1..k].
	
	for(int u=0;u<n;u++){
		int T_u[num_nodes]={0}; //t_u[v]0
		int R_u[num_nodes]={0}; //R_u[v]1
		int L_u[num_nodes][k+1]={0}; //L_u_v_t;
		initialisation();
		
		for(int t=2;t<=k;t++){
			for(int v=m[u]+1;v<u;v++){
				if(v==m[v]){
					//L_u_v_t-1 calculation
					int a = L_u[m_dash[v]][t-1] + R_u[m_dash[v]] + T_u[m_dash[v]] - R_u[v] - T_u[v]; //Au,v,t = L(u,m'v,t) + R(u,m'v)1 + T(u,m'v,0) − R(u,v,1) − T(u,v,0)
					int b = INT_MAX;
					for(int x=m_dash[v];x<m[v];x++){
						for(int t_dash = 1;t_dash<t && t_dash<=v-m[v]+1 && t-(m[v]-m[u])-1<=t_dash ;t_dash++){
							b=min(b,T[x][t_dash] + L_u[x][t-1-t_dash] + R_u[x] - R_u[v] - T_u[v]);
						}
					}
					L_u[v][t-1] = min(a,b);
				}
			}
			T[u][t]=INT_MAX;
			for(int v=m[u];v<u;v++){
				for(int t_dash = 1;t_dash<t && t_dash<=v-m[v]+1 && t-(m[v]-m[u])-1<=t_dash ;t_dash++){
					T[u][t]=min(T[u][t],T[v][t_dash] + L_u[m[v]][t-1-t_dash] + R_u[v] );
				}
			}
		}
		
	}
	
	return 0;
}
