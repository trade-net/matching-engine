#include <gtest/gtest.h>
#include <order_book.h>
#include <order.h>
#include <memory>

TEST(OrderBookTest, AddOrderTest) {
    OrderBook orderBook;

    auto order1 = Order(1, true, 10, 100, 1, "AAPL");
    auto order2 = Order(2, true, 5, 101, 2, "AAPL");

    orderBook.addOrder(order1);
    orderBook.addOrder(order2);

    EXPECT_TRUE(orderBook.isOrderInMap(1));
    EXPECT_TRUE(orderBook.isOrderInMap(2));
    EXPECT_TRUE(orderBook.isLimitInMap(100));
    EXPECT_TRUE(orderBook.isLimitInMap(101));
}

TEST(OrderBookTest, MatchOrderTest) {
    OrderBook orderBook;

    auto buyOrder = Order(1, true, 10, 100, 1, "AAPL");
    auto sellOrder = Order(2, false, 5, 100, 2, "AAPL");

    orderBook.addOrder(buyOrder);
    auto status = orderBook.processOrder(sellOrder);

    EXPECT_EQ(status.unitsFilled, 5);
    EXPECT_EQ(status.unitsUnfilled, 0);
    EXPECT_EQ(status.priceFilled, 500);
    EXPECT_TRUE(orderBook.isOrderInMap(1));
    EXPECT_FALSE(orderBook.isOrderInMap(2));
}

TEST(OrderBookTest, PartialMatchTest) {
    OrderBook orderBook;

    auto buyOrder = Order(1, true, 10, 100, 1, "AAPL");
    auto sellOrder = Order(2, false, 15, 100, 2, "AAPL");

    orderBook.addOrder(buyOrder);
    auto status = orderBook.processOrder(sellOrder);

    EXPECT_EQ(status.unitsFilled, 10);
    EXPECT_EQ(status.unitsUnfilled, 5);
    EXPECT_EQ(status.priceFilled, 1000);
    EXPECT_FALSE(orderBook.isOrderInMap(1));
    EXPECT_TRUE(orderBook.isOrderInMap(2));
    EXPECT_TRUE(orderBook.isLimitInMap(100));
}

TEST(OrderBookTest, NoMatchTest) {
    OrderBook orderBook;

    auto buyOrder = Order(1, true, 10, 100, 1, "AAPL");
    auto sellOrder = Order(2, false, 5, 101, 2, "AAPL");

    orderBook.addOrder(buyOrder);
    auto status = orderBook.processOrder(sellOrder);

    EXPECT_EQ(status.unitsFilled, 0);
    EXPECT_EQ(status.unitsUnfilled, 5);
    EXPECT_EQ(status.priceFilled, 0);
    EXPECT_TRUE(orderBook.isOrderInMap(1));
    EXPECT_TRUE(orderBook.isOrderInMap(2));
}

TEST(OrderBookTest, AddAfterPartialMatchTest) {
    OrderBook orderBook;

    auto buyOrder = Order(1, true, 10, 100, 1, "AAPL");
    auto sellOrder = Order(2, false, 15, 100, 2, "AAPL");

    orderBook.addOrder(buyOrder);
    auto status = orderBook.processOrder(sellOrder);

    EXPECT_EQ(status.unitsInBook, 5);
    EXPECT_TRUE(orderBook.isOrderInMap(2));
    EXPECT_TRUE(orderBook.isLimitInMap(100));
}

TEST(OrderBookTest, TimePriorityWithinPriceLevelTest) {
    OrderBook orderBook;

    // two resting buy orders at the same price; earlier timestamp should fill first
    auto firstBuy = Order(1, true, 5, 100, 1, "AAPL");
    auto secondBuy = Order(2, true, 5, 100, 2, "AAPL");

    orderBook.addOrder(firstBuy);
    orderBook.addOrder(secondBuy);

    auto sellOrder = Order(3, false, 5, 100, 3, "AAPL");
    auto status = orderBook.processOrder(sellOrder);

    EXPECT_EQ(status.unitsFilled, 5);
    EXPECT_FALSE(orderBook.isOrderInMap(1));
    EXPECT_TRUE(orderBook.isOrderInMap(2));
}

TEST(OrderBookTest, MarketBuyOrderMatchesTest) {
    OrderBook orderBook;

    auto sellOrder = Order(1, false, 10, 100, 1, "AAPL");
    auto marketBuyOrder = Order(2, true, 5, 0, 2, "AAPL", /*isMarket=*/true);

    orderBook.addOrder(sellOrder);
    auto status = orderBook.processOrder(marketBuyOrder);

    EXPECT_EQ(status.unitsFilled, 5);
    EXPECT_EQ(status.unitsUnfilled, 0);
    EXPECT_EQ(status.priceFilled, 500);
    EXPECT_FALSE(orderBook.isOrderInMap(2));
}

TEST(OrderBookTest, CancelOrderRemovesRestingOrderTest) {
    OrderBook orderBook;

    auto order = Order(1, true, 10, 100, 1, "AAPL");
    orderBook.addOrder(order);

    EXPECT_TRUE(orderBook.cancelOrder(1));
    EXPECT_FALSE(orderBook.isOrderInMap(1));
    EXPECT_FALSE(orderBook.isLimitInMap(100));
    EXPECT_FALSE(orderBook.isActive());
}

TEST(OrderBookTest, CancelOrderLeavesRestOfLimitIntactTest) {
    OrderBook orderBook;

    auto firstBuy = Order(1, true, 5, 100, 1, "AAPL");
    auto secondBuy = Order(2, true, 5, 100, 2, "AAPL");
    orderBook.addOrder(firstBuy);
    orderBook.addOrder(secondBuy);

    EXPECT_TRUE(orderBook.cancelOrder(1));
    EXPECT_FALSE(orderBook.isOrderInMap(1));
    EXPECT_TRUE(orderBook.isOrderInMap(2));
    EXPECT_TRUE(orderBook.isLimitInMap(100));

    auto sellOrder = Order(3, false, 5, 100, 3, "AAPL");
    auto status = orderBook.processOrder(sellOrder);

    EXPECT_EQ(status.unitsFilled, 5);
}

TEST(OrderBookTest, CancelUnknownOrderReturnsFalseTest) {
    OrderBook orderBook;

    EXPECT_FALSE(orderBook.cancelOrder(999));
}

TEST(OrderBookTest, BestPriceIsEmptyOnEmptyBookTest) {
    OrderBook orderBook;

    EXPECT_FALSE(orderBook.getHighestBuy().has_value());
    EXPECT_FALSE(orderBook.getLowestSell().has_value());
}

TEST(OrderBookTest, UnfilledMarketOrderDoesNotRestTest) {
    OrderBook orderBook;

    auto marketBuyOrder = Order(1, true, 5, 0, 1, "AAPL", /*isMarket=*/true);
    auto status = orderBook.processOrder(marketBuyOrder);

    EXPECT_EQ(status.unitsFilled, 0);
    EXPECT_EQ(status.unitsUnfilled, 5);
    EXPECT_EQ(status.unitsInBook, 0);
    EXPECT_FALSE(orderBook.isOrderInMap(1));
    EXPECT_FALSE(orderBook.isActive());
}
