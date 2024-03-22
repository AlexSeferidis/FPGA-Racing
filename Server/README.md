# Server Documentation

## Overview
Our gaming server and related cloud functionalities are managed using AWS, leveraging a variety of its services. AWS Lambda functions do most of the work, handling both internal and external API requests. External requests are routed through AWS API Gateway, while internal requests are invoked by some external functions. Player data is efficiently managed with AWS DynamoDB, ensuring seamless integration with Lambda.

Lambda was selected for its reliability, simplicity, and cost-effectiveness, offering free service for up to 1 million requests annually. Detailed summaries and test request bodies are provided for each Lambda function for ease of understanding and testing.

## Lambda Functions

### Update Player
- **Type**: External API Call
- **Purpose**: Updates player data in the DynamoDB table. Uses playerID as the primary key and records their position in a race.
- **Trigger**: Game client

### Exchange Code for Tokens
- **Type**: Internal Process
- **Purpose**: Initiated during Cognito login to generate unique tokens for individual users.

### Cognito Login
- **Type**: Authentication Process
- **Purpose**: Facilitates username-password-based login for testing, bypassing standard tokenized authentication. Returns necessary authentication tokens.

### Retrieve New Token
- **Type**: Token Management
- **Purpose**: Accepts a user's refresh token to issue a new authentication token. Essential as tokens expire every 3600 seconds.

### Track Events
- **Type**: Logging Function
- **Purpose**: Captures and logs events and interactions for debugging and testing purposes, utilizing CloudWatch.

### Invalidate Login
- **Type**: External API Call
- **Purpose**: Invalidates all authentication tokens upon user sign-out. Requires generation of new tokens for subsequent logins.

### Get Player Data
- **Type**: External API Request
- **Purpose**: Retrieves player information from the players DynamoDB table.

### Validate Duplicate Email
- **Type**: User Management
- **Purpose**: Ensures uniqueness of user emails during Cognito sign-up to prevent duplicate accounts.

## Deprecated Functions
The following functions have been deprecated and are no longer in active use:

1. **Stop Matchmaking**: Previously used to cancel a player's matchmaking request.
2. **Poll Matchmaking**: Checked for duplicate matchmaking instances.
3. **Start Matchmaking**: Handled game client requests for beginning matchmaking.
4. **Get Sign In Result**: Used during login to indicate success or failure.
5. **GameLift-Start Game Session**: Manually initiated game sessions.

These functions have been kept for archival purposes and may be subject to removal in future updates. Users are advised to refer to the current functional suite for their needs.
