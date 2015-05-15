#!/usr/bin/python
#######################################
# This script takes one ID from the command line and will convert all (one) matching lineitems' lineItemType.
# Should be easy to modify for other changes or more general queries.
# Based off: https://github.com/googleads/googleads-python-lib/blob/master/examples/dfp/v201502/line_item_service/update_line_items.py
# Schema reference: https://developers.google.com/doubleclick-publishers/docs/reference/v201502/LineItemService
#######################################

# Import appropriate modules from the client library.
from googleads import dfp
import datetime
import json
import sys
from time import gmtime, strftime

def main(client, lineid):
  print strftime("%Y-%m-%d %H:%M:%S", gmtime()),"Lineitem ID:", lineid

  # Initialize appropriate service.
  line_item_service = client.GetService('LineItemService', version='v201502')

  # Create statement object to only select line items with even delivery rates.
  # Pass the input parameter [lineid] into the "query" for the API
  # This essential translates the query into "select * from lineitem_service where Id=$lineid"
  values = [{'key': 'id','value': {'xsi_type': 'NumberValue','value': lineid}}]
  query = 'WHERE Id = :id'
  statement = dfp.FilterStatement(query, values, 10)

  # Get line items by statement.
  response = line_item_service.getLineItemsByStatement(statement.ToStatement())
  # If lineitem with ID successfully found:
  if 'results' in response:
    # Update each local line item by changing its delivery rate type.
    updated_line_items = []
    for line_item in response['results']:
      line_item['lineItemType'] = 'PRICE_PRIORITY'
      updated_line_items.append(line_item)

    # Update line items remotely.
    line_items = line_item_service.updateLineItems(updated_line_items)

    # Display results.
    if line_items:
      for line_item in line_items:
        print strftime("%Y-%m-%d %H:%M:%S", gmtime()),'Line item with id',line_item['id'],'updated to', line_item['lineItemType']
    else:
      print 'No line items were updated.'
  else:
    print 'No line items found to update.'

if __name__ == '__main__':
  # Initialize client object.
  client = dfp.DfpClient.LoadFromStorage('googleads.yaml')
  lineid = sys.argv[1]
  main(client, lineid)
