all: $(wildcard *.cpp)
	c++ -std=c++11 -I../include -L../lib ExampleProducer.cpp -o example_producer -lons -Wl,-rpath,../lib
	c++ -std=c++11 -I../include -L../lib ExamplePushConsumer.cpp -o example_push_consumer -lons -Wl,-rpath,../lib
	c++ -std=c++11 -I../include -L../lib ExampleOrderProducer.cpp -o example_order_producer -lons -Wl,-rpath,../lib
	c++ -std=c++11 -I../include -L../lib ExampleOrderPushConsumer.cpp -o example_order_push_consumer -lons -Wl,-rpath,../lib
	c++ -std=c++11 -I../include -L../lib SendThenReceive.cpp -o send_then_receive -lons -pthread -Wl,-rpath,../lib

clean:
	rm -fr example_producer
	rm -fr example_push_consumer
	rm -fr example_order_producer
	rm -fr example_order_push_consumer
	rm -fr send_then_receive