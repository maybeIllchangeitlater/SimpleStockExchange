CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

DROP TABLE user_info IF EXISTS
DROP TABLE bid_info IF EXISTS
DROP TABLE transaction_info IF EXISTS

CREATE TABLE user_info (
                           id UUID PRIMARY KEY NOT NULL DEFAULT uuid_generate_v4(),
                           username VARCHAR(255) NOT NULL,
                           password VARCHAR(255) NOT NULL,
                           balance DECIMAL(10, 2) NOT NULL
);

CREATE TABLE bid_info (
                          id UUID PRIMARY KEY NOT NULL DEFAULT uuid_generate_v4(),
                          seller_id UUID REFERENCES user_info(id) ON DELETE CASCADE,
                          buyer_id UUID REFERENCES user_info(id) ON DELETE CASCADE,
                          rate DECIMAL(10, 2) NOT NULL,
                          quantity INT NOT NULL,
                          time TIMESTAMP NOT NULL
);

CREATE TABLE transaction_info (
                                  id UUID PRIMARY KEY NOT NULL DEFAULT uuid_generate_v4(),
                                  seller_id UUID REFERENCES user_info(id) ON DELETE SET NULL,
                                  buyer_id UUID REFERENCES user_info(id) ON DELETE SET NULL,
                                  rate DECIMAL(10, 2) NOT NULL,
                                  quantity INT NOT NULL,
                                  time TIMESTAMP NOT NULL
);

