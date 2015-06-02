Requirement:

1. Lucky draw starts with Javascripts which triggers a ajax call to backend
2. Each lucky draw will have the result already calculated at backend and send to ajax return
3. Javascript will based on the backend result to stop at that particular region

Lucky draw - Wheel:

Concepts:

1. Lucky draw items
 - Each item describes a prize, hit rates (1%, 2%, 3% and etc), weight

2. Lucky draw entity
 - Contains the lucky draw items
 - Name, type

Entities:

1. Luckydraw - A single luckydraw contains many items to represent a luckydraw player
2. Luckydraw Item - A specific settings for the item, such as prize, rates and etc.
