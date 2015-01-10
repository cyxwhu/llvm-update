BasicBlock *ConFix::confixSplitEdge(BasicBlock *BB, BasicBlock *Succ, Pass *P,
                                    int &iCritical) {
  unsigned SuccNum = GetSuccessorNumber(BB, Succ);
 
  // If this is a critical edge, let SplitCriticalEdge do it.
  TerminatorInst *LatchTerm = BB->getTerminator();
  if (SplitCriticalEdge(LatchTerm, SuccNum, P)) {
    iCritical = 1;
    return LatchTerm->getSuccessor(SuccNum);
  }

  // If the edge isn't critical, then BB has a single successor or Succ has a
  // single pred.  Split the block.
  BasicBlock::iterator SplitPoint;
  if (BasicBlock *SP = Succ->getSinglePredecessor()) {
    // If the successor only has a single pred, split the top of the successor
    // block.
    assert(SP == BB && "CFG broken");
    SP = NULL;
    iCritical = 2;
    return SplitBlock(Succ, Succ->begin(), P);
  } else {
    // Otherwise, if BB has a single successor, split it at the bottom of the
    // block.
    assert(BB->getTerminator()->getNumSuccessors() == 1 &&
           "Should have a single succ!");
    iCritical = 3;
    return SplitBlock(BB, BB->getTerminator(), P);
  }
}
