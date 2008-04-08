class CPolygonNode {
public:
	unsigned long m_next, m_prev;
	unsigned long m_index;
	inline bool removed() const { return m_next + m_prev == 0; }
	inline CPolygonNode() { m_next = m_prev = 0; m_index = 0; }
};

void circularLinkNodes(CPolygonNode *first, unsigned long nVertices, unsigned long offset, unsigned long vertoffs)
{
	assert (nVertices >= 3);

	unsigned long prev = offset + nVertices - 1;
	unsigned long next = offset + 1;

	unsigned long curr;
	
	for ( curr = 0; curr < nVertices-1; ++curr ) {
		first[curr + offset].m_prev  = prev + offset;
		first[curr + offset].m_next  = next + offset;
		first[curr + offset].m_index = curr + vertoffs;
		prev = curr;
		++next;
	}

	first[curr].prev = prev + offset;
	first[curr].next = offset;
}

void triangulatePolygon(RtInt nloops, const RtInt loops[], const RtInt verts[], const RtFloat *p, std::vector<RtInt> &triangles)
{
	/*
		Parameters
		nloops   : number of loops (size of loops)
		loops    : number of vertices per polygon, first loop is the border, others are holes
		verts    : indices of the vertices
		p        : (varying) positions of the vertices indexed by verts
		Returns
		triangle : indirect indexes for triangles of the varying variables. Can be used for face varying as direct indexes.
	*/
	triangles.clear();
	if ( nloops < 0 )
		return;
	unsigned long nVertices = loops[0];
	assert(nVertices >= 3);
	unsigned long nTriangles = (nVertices - 2) + (nloops-1)*2;
	triangles.resize(nTriangles * 3);
	
	unsigned long sumLoops = sum(nloops, loops);
	CPolygonNode *pn = new CPolygonNode[sumLoops + 2*(nloops-1) + 1]; // element 0 is not used
	
	// Insert border and hole indirect indexes, link nodes 0 : 1 : 2  .... : n-1 : 0
	
	unsigned long border = 1;
	unsigned long holes = nloops > 1 ? new (unsigned long)[nloops-1] : 0;
	
	circularLinkNodes(pn, nVertices, border, 0);
	
	unsigned long indexcount = 1 + nVertices;
	unsigned long vertexcount = nVertices;
	unsigned long i;
	
	for ( i = 1; i < nloops; ++i ) {
		indexcount += 2; // bridge edges from border to hole and back to border
		holes[i-1] = indexcount;
		circularLinkNodes(pn, loops[i], holes[i-1], vertexcount);
		indexcount  += loops[i];
		vertexcount += loops[i];
	}
	
	delete[] pn;
	if ( holes )
		delete[] holes;
}
