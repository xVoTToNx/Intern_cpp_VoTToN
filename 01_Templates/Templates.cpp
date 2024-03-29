#include <iostream>
#include <map>
#include <tuple>
#include <functional>
#include <string>
#include <utility>
#include <type_traits>

template <typename Result, typename... Args>
class Cache
{
public:
	std::map<std::tuple<Args...>, Result> m;
	std::function<Result(Args...)> f;
	Cache(std::function<Result(Args...)> f_) : f{ f_ } {};

	Cache(std::string&& f_) : f{ f_ } {};

	Result find(Args... args)
	{
		auto t = std::make_tuple(args...);
		auto ft = m.find(t);
		if (ft != m.end())
			return m[t];
		Result r = f(std::forward<Args...>(args...));
		m.insert(std::make_pair(t, r));
		return r;
	}

};

std::string getIpInput(std::string str)
{
	return str;
}

std::string getIp(std::string const& s)
{
	static Cache<std::string, std::string> cache(getIpInput);
	return cache.find(s);
}


int main()
{
    std::cout << "Hello World!\n";
	std::cout << getIp("One") << std::endl;
	std::cout << getIp("One") << std::endl;
}
