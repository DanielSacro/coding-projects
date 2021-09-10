// ---------------------------------------------------
// Name: Daniel Sacro
// CMPUT 275, Winter 2021
//
// Major Assignment 1: Navigation System (Individual)
// ---------------------------------------------------
#include <iostream>
#include "wdigraph.h"
#include <utility>
#include <unordered_map>
#include <vector>
#include <queue>
using namespace std;

// Compile error if these pair definitions aren't provided

/* 	PIL is an int-long long int pair where the first element
 	is the vertex u (where the fire comes from) and the second
	element is d (the time taken to travel from there) */
typedef pair<int, long long> PIL;

/*	PIPIL is an int-PIL pair where the first element is the
	vertex v (where the fire travels to) and the second
	element is PIL */
typedef pair<int, PIL> PIPIL;

class CompGreater
{
	/* 	Description: Comparison class for the priority queue template.
					 Effectively makes the priority queue a min heap.

		Arguments: None.

		Returns: Nothing.
	*/
public:
	bool operator()(const PIPIL &lhs, const PIPIL &rhs) const
	{
		// min heap using distance/weight values in PIPIL
		return (lhs.second.second > rhs.second.second);
	}
};

void dijkstra(const WDigraph &graph, int startVertex, unordered_map<int, PIL> &tree)
{
	/*	Description: Uses dijkstra's algorithm to compute least cost paths starting 
					 from a given vertex (startVertex). Uses priority_queue to act
					 as a min heap.

		Arguments:
			graph (reference to WDigraph object): The graph containing all edges and
												  vertices for Edmonton
			startVertex (int): The starting vertex or point of reference
			tree (reference to unordered map of int-PIL pair):
				-> A search tree used to construct the least cost path to some vertex

		Returns: Nothing.

	*/
	// "Min heap" contains all active fires
	priority_queue<PIPIL, vector<PIPIL>, CompGreater> fires;

	// Fire starts at the startVertex.
	// -1 is used since the fire does not come
	// from any vertices before startVertex
	fires.push(PIPIL(startVertex, PIL(-1, 0)));

	// While there is an active fire
	while (!fires.empty())
	{
		// Find the fire that reaches endpoint v the earliest
		auto earliestFire = fires.top();

		// Make variables more readable
		// v is the vertex reached, u is the vertex the fire came from, d is time taken
		int v = earliestFire.first, u = earliestFire.second.first, d = earliestFire.second.second;

		// No fire at u anymore
		fires.pop();

		// If v is already burned/reached, don't do anything
		if (tree.find(v) != tree.end())
		{
			continue;
		}

		// Otherwise, declare v burned at time d with a fire coming from u
		tree[v] = PIL(u, d);

		// Fire spreads across all edges exiting vertex v
		for (auto it = graph.neighbours(v); it != graph.endIterator(v); it++)
		{
			int nbr = *it;

			// Fire starts at v at time d and reaches
			// nbr at time d + (length of v->nbr edge)
			long long burn = d + graph.getCost(v, nbr);
			fires.push(PIPIL(nbr, PIL(v, burn)));
		}
	}
}
