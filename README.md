# Matching Engine
This is the core service of the stock exchange simulation, which maintains the limit order book for all instruments and processes incoming order requests.

## Order Book Architecture
Each `OrderBook` consists of 2 maps to store buy and sell limit orders respectively.
Each key-value pair in the map stores the limit price and the `Limit` object respectively.
Each `Limit` contains a list of all `Order`s at that price.

To provide O(1) time complexity access to each `Limit` and `Order`, the `OrderBook` also contains 2 additional unordered_maps,
mapping the limit and order id to the map iterator where the relevant `Limit` is stored.
The `Limit` class also has an unordered_map that maps order ids to the list iterator of the relevant `Order`.

## Processing Incoming Orders
Currently, this service is designed to be an Async TCP server that accepts orders via incoming `orderRequests` from its clients.
When the matching engine receives an order request, it enqueues the task to process the order onto an instrument-partitioned threadpool.
The threadpool is partitioned by instrument to ensure that orders for a given instrument is only matched by a single thread.
When the task is picked up, the matcher will attempt to match the incoming order with existing orders in the book.
If the incoming order is a limit order, and it is still unfilled/partially filled after that, the order is then also added to the order book.

However, the plan is for the matching engine to consume order events from a new sequencer service via UDP multicast in the future.
The matcher will then be converted into a speculative matching system, which proposes trade events to the sequencer, and have them accepted, before completing its matches.
This would ensure determinism in the simulation.

## Future Work
- Improve logging (currently "logs" are just printed on std::cout)
- Profile the service
- Update the service to consume events from a sequencer instead
