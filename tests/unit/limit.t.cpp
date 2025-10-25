#include <gtest/gtest.h>
#include <limit.h>
#include <order.h>
#include <memory>

TEST(LimitTest, ConstructorInitializesCorrectly) {
    auto order = Order(1, true, 100, 50, 123456, "AAPL");
    Limit limit(order);

    EXPECT_EQ(limit.price(), 50);
    EXPECT_EQ(limit.volume(), 100);
    EXPECT_EQ(limit.size(), 1);
}

TEST(LimitTest, AddOrderToLimit) {
    auto order1 = Order(1, true, 100, 50, 123456, "AAPL");
    auto order2 = Order(2, true, 200, 50, 123457, "AAPL");
    Limit limit(order1);

    limit.addOrderToLimit(order2);

    EXPECT_EQ(limit.volume(), 300);
    EXPECT_EQ(limit.size(), 2);
}

TEST(LimitTest, FillUnits) {
    auto order1 = Order(1, true, 100, 50, 123456, "AAPL");
    auto order2 = Order(2, true, 200, 50, 123457, "AAPL");
    Limit limit(order1);
    limit.addOrderToLimit(order2);

    auto filledOrders = limit.fillUnits(150);

    EXPECT_EQ(filledOrders.size(), 1);
    EXPECT_EQ(filledOrders[0], 1);
    EXPECT_EQ(limit.volume(), 150);
    EXPECT_EQ(limit.size(), 1);
}

TEST(LimitTest, DecrementHeadOrder) {
    auto order = Order(1, true, 100, 50, 123456, "AAPL");
    Limit limit(order);

    limit.decrementHeadOrder(50);

    EXPECT_EQ(limit.volume(), 50);
    EXPECT_EQ(order.units, 100);
}

TEST(LimitTest, GetOrders) {
    auto order1 = Order(1, true, 100, 50, 123456, "AAPL");
    auto order2 = Order(2, true, 200, 50, 123457, "AAPL");
    Limit limit(order1);
    limit.addOrderToLimit(order2);

    auto orders = limit.getOrders();

    EXPECT_EQ(orders.size(), 2);
    EXPECT_NE(std::find(orders.begin(), orders.end(), 1), orders.end());
    EXPECT_NE(std::find(orders.begin(), orders.end(), 2), orders.end());
}

TEST(LimitTest, FindOrder) {
    auto order1 = Order(1, true, 100, 50, 123456, "AAPL");
    auto order2 = Order(2, true, 200, 50, 123457, "AAPL");
    Limit limit(order1);
    limit.addOrderToLimit(order2);

    EXPECT_TRUE(limit.find(1));
    EXPECT_TRUE(limit.find(2));
    EXPECT_FALSE(limit.find(3));
}
