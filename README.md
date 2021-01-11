# crypto_ds
C++ client-side program to interact with FTX.com api and populate MongoDB database for data analysis.

**This repo achieves the following functions:**
- Establish connection with FTX.com API
- Fetch market and orderbook data from a specific market
- Manage and execute trades
- Populate database with historical and real time market data

## To Start with crypto_ds

**1. Set up required Dependencies**

**Required Dependencies:**
- Curl & libcurl library
- [rapidjson](https://rapidjson.org/)
- Running Mongodb instance
- [libmongoc](http://mongoc.org/libmongoc/current/index.html)

**2. Compile using CMake, and Run**
Save the key and secret of FTX.com api in a two line file.
```
// Inside ftx_key.txt
[key]
[secret]
```

From the top level folder, run the following commands:
```
cmake .
make

// Run
./autot
```

## Further Development Tasks
- Processing of higher density & resolution data to analyze orderbook microstructure