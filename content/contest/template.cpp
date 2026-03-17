ll cdiv(ll a, ll b) { return a/b+((a^b)>0&&a%b); }
ll fdiv(ll a, ll b) { return a/b-((a^b)<0&&a%b); }
int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(cin.failbit);
}
