/* User configuration for ESP32 Financial Ticker + Weather Display.
 *
 * EDIT THIS FILE TO CUSTOMIZE YOUR TICKER.
 * All user-editable settings are in one place.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

// ── API Keys ─────────────────────────────────────────────────
// CoinGecko: https://www.coingecko.com/en/api
// Free demo key works fine for 10-30 calls/min
#define COINGECKO_API_KEY   "CG-5QB3M85gSbWvPaYQVS9dzghq"

// OpenWeatherMap: configured in config.cpp (OWM_APIKEY)

// ── Page 1: CRYPTOCURRENCY ──────────────────────────────────
// CoinGecko IDs: bitcoin, ethereum, hyperliquid, cardano,
//   solana, dogecoin, ripple, polkadot, avalanche-2, chainlink
#define CRYPTO_1_ID      "bitcoin"
#define CRYPTO_1_SYMBOL  "BTC"
#define CRYPTO_1_NAME    "Bitcoin"

#define CRYPTO_2_ID      "ethereum"
#define CRYPTO_2_SYMBOL  "ETH"
#define CRYPTO_2_NAME    "Ethereum"

#define CRYPTO_3_ID      "hyperliquid"
#define CRYPTO_3_SYMBOL  "HYPE"
#define CRYPTO_3_NAME    "Hyperliquid"

#define CRYPTO_4_ID      "cardano"
#define CRYPTO_4_SYMBOL  "ADA"
#define CRYPTO_4_NAME    "Cardano"

// ── Page 2: STOCK INDICES ───────────────────────────────────
// Yahoo Finance symbols: ^GSPTSE (TSX), ^IXIC (NASDAQ),
//   ^GSPC (S&P 500), ^DJI (Dow), ^FTSE, ^N225, ^HSI
#define INDEX_1_SYMBOL   "^GSPTSE"
#define INDEX_1_DISPLAY  "TSX"
#define INDEX_1_NAME     "S&P/TSX"

#define INDEX_2_SYMBOL   "^IXIC"
#define INDEX_2_DISPLAY  "NDQ"
#define INDEX_2_NAME     "NASDAQ"

#define INDEX_3_SYMBOL   "^GSPC"
#define INDEX_3_DISPLAY  "S&P"
#define INDEX_3_NAME     "S&P 500"

#define INDEX_4_SYMBOL   "^DJI"
#define INDEX_4_DISPLAY  "DJIA"
#define INDEX_4_NAME     "Dow Jones"

// ── Page 3: COMMODITIES ─────────────────────────────────────
// Yahoo Finance futures: GC=F (Gold), SI=F (Silver),
//   CL=F (WTI Oil), KC=F (Coffee), HG=F (Copper),
//   NG=F (Natural Gas), PL=F (Platinum), ZW=F (Wheat)
#define COMMODITY_1_SYMBOL   "GC=F"
#define COMMODITY_1_DISPLAY  "AU"
#define COMMODITY_1_NAME     "Gold"
#define COMMODITY_1_UNIT     "/oz"

#define COMMODITY_2_SYMBOL   "SI=F"
#define COMMODITY_2_DISPLAY  "AG"
#define COMMODITY_2_NAME     "Silver"
#define COMMODITY_2_UNIT     "/oz"

#define COMMODITY_3_SYMBOL   "CL=F"
#define COMMODITY_3_DISPLAY  "OIL"
#define COMMODITY_3_NAME     "Crude Oil"
#define COMMODITY_3_UNIT     "/bbl"

#define COMMODITY_4_SYMBOL   "KC=F"
#define COMMODITY_4_DISPLAY  "KC"
#define COMMODITY_4_NAME     "Coffee"
#define COMMODITY_4_UNIT     "/lb"

// ── Page 4: CURRENCIES ──────────────────────────────────────
// Yahoo Finance forex: CADUSD=X, EURUSD=X, JPY=X, GBPUSD=X,
//   AUDUSD=X, NZDUSD=X, CHFUSD=X, CNYUSD=X
#define FX_1_SYMBOL   "CADUSD=X"
#define FX_1_DISPLAY  "$/$"
#define FX_1_NAME     "USD / CAD"

#define FX_2_SYMBOL   "EURUSD=X"
#define FX_2_DISPLAY  "E/$"
#define FX_2_NAME     "EUR / USD"

#define FX_3_SYMBOL   "JPY=X"
#define FX_3_DISPLAY  "$/Y"
#define FX_3_NAME     "USD / JPY"

#define FX_4_SYMBOL   "GBPUSD=X"
#define FX_4_DISPLAY  "L/$"
#define FX_4_NAME     "GBP / USD"

// ── Timing ──────────────────────────────────────────────────
#define PAGE_DURATION_SEC   60   // Seconds per page (60 = 1 min)
#define TOTAL_PAGES         5    // Number of pages in rotation
// Data refreshes every full cycle: PAGE_DURATION_SEC * TOTAL_PAGES

// ── Display ─────────────────────────────────────────────────
#define CURRENCY_PRIMARY     "CAD"   // For commodity/crypto display
#define USD_TO_CAD_FALLBACK  1.42f   // Used if forex fetch fails

// ── CoinGecko Settings ──────────────────────────────────────
#define COINGECKO_VS_CURRENCY "cad"  // Currency for crypto prices

#endif
