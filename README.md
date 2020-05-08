Q project

In this project:

An MTA is developing a system for the usage of commuter passes. There are currently four types of transportation:
  1) Bus
  2) Subway
  3) Commuter Rail
  4) Special Bus (For Elderly Riders)

A vending machine distributes these passes. There are two types of passes: Monthly and Pay-per-ride. Monthly are as the name implied, monthly passes that will allow the traveller to have unlimited access to their preferred mode of transportation.
Pay per ride passes will have a balance and the user will be charged per ride.

The prices have a different base price depending on the mode of transportation. Students and Elderly get a 50% discount on both types of passes. Monthly passes are prorated to 50% off after the 15th of the month. Weekend ride prices are 75% of the weekday prices. 

In the future, prices for single rides on trains and subway will be based on the length of the trip.



- User should be able to buy a pass
  - Should be able to specify if user is a student or if user is elderly
  - Should be able to select pass type (monthly or pay­per­ride)
  - If Monthly
    + Select mode of transport type
    + Display price to purchase
  - If pay-per-ride
    + Allow user to enter amount purchasing

- User should be able to use the pass
  - Select mode of transport type
  - If pass is pay-per-ride
    + Verify pass has sufficient credit to for the chosen transport type
    + Deduct price of transport type from the pass
  - If monthly
    + Verify pass is valid for this type and is active
    
- User should be able to check the balance on the card
