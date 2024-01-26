//http://www.usaco.org/index.php?page=viewproblem2&cpid=970

#include <bits/stdc++.h>
using namespace std;

const int mxN=1e5;
int pos[mxN],heavy[mxN],head[mxN],par[mxN],subtree[mxN],depth[mxN],cur_pos;

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

void hld(int node,int chain,vector<int> adj[],vector<int> &value,vector<int> &a)
{
    head[node]=chain;
    pos[node]=cur_pos++;
    a[pos[node]]=value[node];
    if(heavy[node]!=-1)
    {
        hld(heavy[node],chain,adj,value,a);
    }
    for(auto &v:adj[node])
    {
        if(v!=par[node]&&v!=heavy[node])
        {
            hld(v,v,adj,value,a);
        }
    }
}

struct Query
{
    int idx,lt,rt,blk,cur;
};

bool cmp(Query a,Query b)
{
    if(a.blk==b.blk)
    {
        if(a.blk&1)
        {
            return a.rt<b.rt;
        }
        return a.rt>b.rt;
    }
    return a.blk<b.blk;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    freopen("milkvisits.in","r",stdin);
    freopen("milkvisits.out","w",stdout);
    int n,q,i1=0,i2=-1;
    cin>>n>>q;
    init(n);
    vector<int> adj[n],value(n),a(n),ans(q,0),freq(n+1,0);
    vector<Query> qry;
    for(int i=0;i<n;++i)
    {
        cin>>value[i];
    }
    for(int i=1;i<n;++i)
    {
        int u,v;
        cin>>u>>v;
        adj[u-1].push_back(v-1);
        adj[v-1].push_back(u-1);
    }
    dfs(0,0,adj);
    hld(0,0,adj,value,a);
    for(int i=0;i<q;++i)
    {
        int u,v,type=-1;
        Query temp;
        cin>>u>>v>>type;
        u--;
        v--;
        while(head[u]!=head[v])
        {
            if(depth[head[u]]<depth[head[v]])
            {
                swap(u,v);
            }
            temp.lt=pos[head[u]];
            temp.rt=pos[u];
            temp.blk=pos[head[u]]/sqrtl(n);
            temp.idx=i;
            temp.cur=type;
            qry.push_back(temp);
            u=par[head[u]];
        }
        if(depth[u]<depth[v])
        {
            swap(u,v);
        }
        temp.lt=pos[v];
        temp.rt=pos[u];
        temp.blk=pos[v]/sqrtl(n);
        temp.idx=i;
        temp.cur=type;
        qry.push_back(temp);
    }
    sort(qry.begin(),qry.end(),cmp);
    for(auto &v:qry)
    {
        while(v.lt<i1)
        {
            freq[a[--i1]]++;
        }
        while(v.rt>i2)
        {
            freq[a[++i2]]++;
        }
        while(v.lt>i1)
        {
            freq[a[i1++]]--;
        }
        while(v.rt<i2)
        {
            freq[a[i2--]]--;
        }
        if(freq[v.cur]>0)
        {
            ans[v.idx]=1;
        }
    }
    for(auto &v:ans)
    {
        cout<<v;
    }
    cout<<'\n';
    return 0;
}
