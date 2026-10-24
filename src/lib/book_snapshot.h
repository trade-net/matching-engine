#ifndef INCLUDED_BOOK_SNAPSHOT
#define INCLUDED_BOOK_SNAPSHOT

#include <cstdint>
#include <iomanip>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

struct LevelSnapshot{
	int price;
	int volume; // units resting at this price
	int orders;
};

// A point-in-time copy of one book. bids/asks hold only the best `depth`
// levels (best first), the totals cover the whole book.
struct BookSnapshot{
	std::vector<LevelSnapshot> bids;
	std::vector<LevelSnapshot> asks;
	size_t bidLevels = 0;
	size_t askLevels = 0;
	size_t restingOrders = 0;

	std::optional<int> bestBid() const{
		return bids.empty() ? std::nullopt : std::optional<int>(bids.front().price);
	}

	std::optional<int> bestAsk() const{
		return asks.empty() ? std::nullopt : std::optional<int>(asks.front().price);
	}

	std::optional<int> spread() const{
		if(bids.empty() or asks.empty()) return std::nullopt;
		return asks.front().price - bids.front().price;
	}
};

inline std::string formatExchangeState(const std::map<std::string, BookSnapshot>& books)
{
	size_t restingOrders = 0;
	for(const auto& [security, book] : books) restingOrders += book.restingOrders;

	auto price = [](std::optional<int> p){
		return p ? std::to_string(*p) : std::string("-");
	};

	const std::string rule(58, '-');
	std::ostringstream out;
	out << "\n" << std::string(20, '=') << " EXCHANGE STATE " << std::string(20, '=') << "\n"
		<< " active books: " << books.size() << "    resting orders: " << restingOrders << "\n"
		<< rule << "\n";

	for(const auto& [security, book] : books){
		out << " " << std::left << std::setw(6) << security
			<< " bid " << price(book.bestBid())
			<< "  ask " << price(book.bestAsk())
			<< "  spread " << price(book.spread())
			<< "  levels " << book.bidLevels << "b/" << book.askLevels << "a"
			<< "  orders " << book.restingOrders << "\n";

		auto level = [&](const char* side, const LevelSnapshot& l){
			out << "        " << side << " " << std::right << std::setw(6) << l.price
				<< " x " << std::setw(5) << l.volume
				<< "  (" << l.orders << (l.orders == 1 ? " order)" : " orders)") << "\n";
		};

		// asks worst-to-best then bids best-to-worst, so the touch sits in the middle
		for(auto it = book.asks.rbegin(); it != book.asks.rend(); ++it) level("ASK", *it);
		for(const auto& l : book.bids) level("BID", l);
		out << rule << "\n";
	}

	return out.str();
}

#endif
