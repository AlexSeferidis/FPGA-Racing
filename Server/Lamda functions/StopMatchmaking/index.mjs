import AWS from 'aws-sdk';

const GameLift = new AWS.GameLift({ region: 'us-east-1' });

export const handler = async (event) => {
  let response;
  let ticketId;

  if (event.body) {
    const body = JSON.parse(event.body);
    if (body.ticketId) {
      ticketId = body.ticketId;
    }
  }

  if (!ticketId) {
    response = {
      statusCode: 400,
      body: JSON.stringify({
        error: 'incoming request did not have a ticket id'
      })
    };
    return response;
  }

  const gameLiftRequestParams = {
    TicketId: ticketId
  };

  try {
    await GameLift.stopMatchmaking(gameLiftRequestParams).promise();
    response = {
      statusCode: 200,
      body: JSON.stringify({
        success: 'matchmaking request has been cancelled'
      })
    };
  } catch (err) {
    response = {
      statusCode: 400,
      body: JSON.stringify({
        error: err.message
      })
    };
  }

  return response;
};
