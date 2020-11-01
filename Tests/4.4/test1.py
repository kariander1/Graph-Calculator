
import graph as gw
#super basic test to check the first 5 functions

graph = gw.create()
gw.addVertex(graph, 'v1')
gw.addVertex(graph, 'v2')
gw.addEdge(graph, 'v1','v2')
gw.disp(graph)
gw.addVertex(graph, 'v3')
gw.addEdge(graph, 'v2','v3')
gw.disp(graph)
gw.destroy(graph)
