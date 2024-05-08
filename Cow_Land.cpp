//https://usaco.org/index.php?page=viewproblem2&cpid=921

#include <bits/stdc++.h>
using namespace std;

const int MAX_N=1e5;
int pos[MAX_N],heavy[MAX_N],head[MAX_N],par[MAX_N],subtree[MAX_N],depth[MAX_N],cur_pos;

void init(int n)
{
    for(int i=0;i<n;++i)
    {
        pos[i]=heavy[i]=-1;
        head[i]=par[i]=i;
        subtree[i]=depth[i]=0;
    }
    cur_pos=0;
}

void dfs(int node,int parent,vector<int> adj[])
{
    subtree[node]=1;
    for(auto &v:adj[node])
    {
        if(v!=parent)
        {
            depth[v]=depth[node]+1;
            par[v]=node;
            dfs(v,node,adj);
            subtree[node]+=subtree[v];
            if(heavy[node]==-1)
            {
                heavy[node]=v;
            }
            else
            {
                if(subtree[heavy[node]]<subtree[v])
                {
                    heavy[node]=v;
                }
            }
        }
    }
}

void hld(int node,int chain,vector<int> adj[],vector<int> &a,vector<int> &value)
{
    head[node]=chain;
    pos[node]=cur_pos++;
    a[pos[node]]=value[node];
    if(heavy[node]!=-1)
    {
        hld(heavy[node],chain,adj,a,value);
    }
    for(auto &v:adj[node])
    {
        if(v!=par[node]&&v!=heavy[node])
        {
            hld(v,v,adj,a,value);
        }
    }
}

class SegmentTree
{
    
private:
    
    vector<int> seg;
    
public:
    
    SegmentTree(int n)
    {
        seg.resize(4*n+1);
    }
    
    void build(int ind,int lo,int hi,vector<int> &a)
    {
        if(lo==hi)
        {
            seg[ind]=a[lo];
            return;
        }
        int mid=lo+(hi-lo)/2;
        build(2*ind+1,lo,mid,a);
        build(2*ind+2,mid+1,hi,a);
        seg[ind]=(seg[2*ind+1]^seg[2*ind+2]);
    }
    
    void update(int ind,int lo,int hi,int i,int val)
    {
        if(lo==hi)
        {
            seg[ind]=val;
            return;
        }
        int mid=lo+(hi-lo)/2;
        if(i<=mid)
        {
            update(2*ind+1,lo,mid,i,val);
        }
        else
        {
            update(2*ind+2,mid+1,hi,i,val);
        }
        seg[ind]=(seg[2*ind+1]^seg[2*ind+2]);
    }
    
    int query(int ind,int lo,int hi,int l,int r)
    {
        if(l>hi||lo>r)
        {
            return 0;
        }
        if(l<=lo&&hi<=r)
        {
            return seg[ind];
        }
        int mid=lo+(hi-lo)/2;
        int left=query(2*ind+1,lo,mid,l,r),right=query(2*ind+2,mid+1,hi,l,r);
        return (left^right);
    }
    
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    freopen("cowland.in","r",stdin);
    freopen("cowland.out","w",stdout);
    int n,q;
    cin>>n>>q;
    init(n);
    vector<int> adj[n],value(n),a(n);
    for(auto &v:value)
    {
        cin>>v;
    }
    for(int i=1;i<n;++i)
    {
        int u,v;
        cin>>u>>v;
        adj[u-1].push_back(v-1);
        adj[v-1].push_back(u-1);
    }
    dfs(0,0,adj);
    hld(0,0,adj,a,value);
    SegmentTree st(n);
    st.build(0,0,n-1,a);
    while(q--)
    {
        int type;
        cin>>type;
        if(type==1)
        {
            int i,val;
            cin>>i>>val;
            i--;
            st.update(0,0,n-1,pos[i],val);
        }
        else
        {
            int u,v,ans=0;
            cin>>u>>v;
            u--;
            v--;
            while(head[u]!=head[v])
            {
                if(depth[head[u]]<depth[head[v]])
                {
                    swap(u,v);
                }
                ans=(ans^st.query(0,0,n-1,pos[head[u]],pos[u]));
                u=par[head[u]];
            }
            if(depth[u]<depth[v])
            {
                swap(u,v);
            }
            ans=(ans^st.query(0,0,n-1,pos[v],pos[u]));
            cout<<ans<<'\n';
        }
    }
    return 0;
}
