# CSCI2270_jpapaioan
Final Project for CSCI 2270

Project Summary: 
This program is a road trip planner facilitated by a graph of nodes of locations across the contiguous United States. The user gives starting and final destinations along with their GPS coordinates in latitude, longitude(ex: 37.5, 121.5), and the program determines the closest cities in the graph and constructs a shortest path route. The user also provides the average number of hours they will be driving per day and estimates the total duration of the trip. You can print the graph to see which cities are included and the program checks to see whether or not your starting or final destinations are already in the graph.

How to Run:
There are three txt files provided that contain the cities, adjacency matrix, and GPS coordinates. You need to enter all three from the command line in order for the program to execute properly.

Example:
./roadtrip cities.txt ad_list.txt ps.txt

Dependencies:
No known dependencies

System Requirements:
none

Group Members:
John Papaioannou

Open issues/bugs:
The program doesn’t check that the user merely chose not to capitalize a starting or final destination when comparing to the cities in the graph, such that “portland” won’t be matched with “Portland” in the graph.

