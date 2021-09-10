# ----------------------------------------------------
#   Name: Daniel Sacro
#   CMPUT 274, Fall 2020
#   
#   Major Assignment #2: Huffman Coding (File Compression)
# ----------------------------------------------------
import bitio
import huffman
import pickle

def read_tree(tree_stream):
    ''' Read a description of a Huffman tree from the given compressed
    tree stream, and use the pickle module to construct the tree object.
    Then, return the root node of the tree itself.

    Args:
      tree_stream: The compressed stream to read the tree from.

    Returns:
      A Huffman tree root constructed according to the given description.
    '''
    tree = pickle.load(tree_stream)
    return tree

def decode_byte(tree, bitreader):
    ''' Reads bits from the bit reader and traverses the tree from
    the root to a leaf. Once a leaf is reached, bits are no longer read
    and the value of that leaf is returned.

    Args:
      bitreader: An instance of bitio.BitReader to read the tree from.
      tree: A Huffman tree.

    Returns:
      Next byte of the compressed bit stream.
    '''
    ''' Written similarly to the "Decode()" function in the "Workbook on
    Trees and Huffman Trees" PDF.
    isinstance compares objects to classes and is used to traverse the
    huffman tree
    '''
    if isinstance(tree,huffman.TreeLeaf):
      return tree.getValue()

    bit = bitreader.readbit()
    if bit:
      return decode_byte(tree.getRight(),bitreader)

    return decode_byte(tree.getLeft(),bitreader)

def decompress(compressed, uncompressed):
    '''First, read a Huffman tree from the 'compressed' stream using your
    read_tree function. Then use that tree to decode the rest of the
    stream and write the resulting symbols to the 'uncompressed'
    stream.

    Args:
      compressed: A file stream from which compressed input is read.
      uncompressed: A writable file stream to which the uncompressed
          output is written.

    Returns:
      Nothing.
    '''
    tree = read_tree(compressed)
    bitReader = bitio.BitReader(compressed)
    bitWriter = bitio.BitWriter(uncompressed)  

    # Written similarly to the code on slide 183 of lecture 20's PDF
    end_of_file = False
    byteList = []
    while not end_of_file:
      try:
        byte = decode_byte(tree,bitReader)
        byteList.append(byte)
      except EOFError:
        end_of_file = True

    try:
      for eachByte in byteList:
        bitWriter.writebits(eachByte,8)
    except:
      bitWriter.flush()
      pass
    bitWriter.flush()

    pass

def write_tree(tree, tree_stream):
    '''Write the specified Huffman tree to the given tree_stream
    using pickle.

    Args:
      tree: A Huffman tree.
      tree_stream: The binary file to write the tree to.

    Returns:
      Nothing.
    '''
    pickle.dump(tree, tree_stream)
    pass

def compress(tree, uncompressed, compressed):
    '''First write the given tree to the stream 'compressed' using the
    write_tree function. Then use the same tree to encode the data
    from the input stream 'uncompressed' and write it to 'compressed'.
    If there are any partially-written bytes remaining at the end,
    write 0 bits to form a complete byte.

    Flush the bitwriter after writing the entire compressed file.

    Args:
      tree: A Huffman tree.
      uncompressed: A file stream from which you can read the input.
      compressed: A file stream that will receive the tree description
          and the coded input data.

    Returns:
      Nothing.
    '''
    write_tree(tree,compressed)
    encodingTable = huffman.make_encoding_table(tree)
    bitReader = bitio.BitReader(uncompressed)
    bitWriter = bitio.BitWriter(compressed)

    # Written similarly to the code on slide 183 of lecture 20's PDF
    byteList = []
    end_of_file = False
    while not end_of_file:
      try:
        byte = bitReader.readbits(8)
        byteList.append(byte)
      except EOFError:
        end_of_file = True

    encodedByteList = []
    try:
      for eachByte in byteList:
        encodedByte = encodingTable[eachByte]
        encodedByteList.append(encodedByte)
      encodedByteList.append(encodingTable[None]) # EOF byte to tell decompressor when to stop
    except:
      pass

    # Files didn't compress properly when writing full bytes, 
    # so bits are written individually.
    for eachEncodedByte in encodedByteList:
      for eachEncodedBit in eachEncodedByte:
        bitWriter.writebit(eachEncodedBit)
    bitWriter.flush()

    pass