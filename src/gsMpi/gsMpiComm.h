/** @file gsMpiComm.h
    
    @brief A wrapper for MPI communicators.
    
    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): C. Hofer -- Code based on ideas from the DUNE library
*/


#pragma once

#include <gsCore/gsForwardDeclarations.h>

namespace gismo
{

/**
  @brief A serial communication class
 
  This communicator can be used if no MPI is available or one wants to run
  sequentially even if MPI is available and used.

  \ingroup Mpi
 */
class gsSerialComm
{
public:
    /**
      @brief return rank of process, i.e. zero
      
      This function is intentionally left non-static to avoid compiler
      warnings of unused object.
     */
    int rank () const { return 0; }

    /**
      @brief return rank of process, i.e. one
     */
    static int size () { return 1; }

    /**
      @brief Returns the name of the communicator
     */
    static std::string name() { return "gsSerialComm"; }


#ifdef GISMO_WITH_MPI
    operator MPI_Comm () const  { return MPI_COMM_SELF;}
#endif

public:

    /** @brief  Compute the sum of the argument over all processes and
        return the result in every process. Assumes that T has an operator+
    */
    template<typename T>
    static T sum (T& in)
    {
        return in;
    }

    /** @brief Compute the sum over all processes for each component
        of an array and return the result in every process. Assumes
        that T has an operator+
    */
    template<typename T>
    static int sum (T* inout, int len)
    {
        return 0;
    }

    /** @brief Compute the product of the argument over all processes
        and return the result in every process. Assumes that T has an
        operator*
    */
    template<typename T>
    static T prod (T& in)
    {
        return in;
    }

    /** @brief Compute the product over all processes for each
        component of an array and return the result in every
        process. Assumes that T has an operator*
    */
    template<typename T>
    static int prod (T* inout, int len)
    {
        return 0;
    }

    /** @brief Compute the minimum of the argument over all processes
        and return the result in every process. Assumes that T has an
        operator<
    */
    template<typename T>
    static T min (T& in)
    {
        return in;
    }

    /** @brief Compute the minimum over all processes
        for each component of an array and return the result
        in every process. Assumes that T has an operator<
    */
    template<typename T>
    static int min (T* inout, int len)
    {
        return 0;
    }

    /** @brief Compute the maximum of the argument over all processes
        and return the result in every process. Assumes that T has an
        operator<
    */
    template<typename T>
    static T max (T& in)
    {
        return in;
    }

    /** @brief Compute the maximum over all processes for each
        component of an array and return the result in every
        process. Assumes that T has an operator<
    */
    template<typename T>
    static int max (T* inout, int len)
    {
        return 0;
    }

    /** @brief Wait until all processes have arrived at this point in
     * the program.
     */
    static int barrier ()
    {
        return 0;
    }

    /** @brief Distribute an array from the process with rank root to
     * all other processes
     */
    template<typename T>
    static int broadcast (T* inout, int len, int root)
    {
        return 0;
    }

    /** @brief  Gather arrays on root task.
     *
     * Each process sends its in array of length len to the root
     * process (including the root itself). In the root process these
     * arrays are stored in rank order in the out array which must
     * have size len * number of processes.  @param[in] in The send
     * buffer with the data to send.  @param[out] out The buffer to
     * store the received data in. Might have length zero on non-root
     * tasks.  @param[in] len The number of elements to send on each
     * task.  @param[in] root The root task that gathers the data.
     */
    template<typename T>
    static int gather (T* in, T* out, int len, int root) // note out must have same size as in
    {
        // copy_n(in, len, out);
        for (int i=0; i<len; i++)
            out[i] = in[i];
        return 0;
    }

    /** @brief  Gather arrays of variable size on root task.
     *
     * Each process sends its in array of length sendlen to the root process
     * (including the root itself). In the root process these arrays are stored in rank
     * order in the out array.
     *
     * @param[in] in The send buffer with the data to be sent
     * @param[in] sendlen The number of elements to send on each task
     * @param[out] out The buffer to store the received data in. May have length zero on non-root
     *                 tasks.
     * @param[in] recvlen An array with size equal to the number of processes containing the number
     *                    of elements to receive from process i at position i, i.e. the number that
     *                    is passed as sendlen argument to this function in process i.
     *                    May have length zero on non-root tasks.
     * @param[out] displ An array with size equal to the number of processes. Data received from
     *                  process i will be written starting at out+displ[i] on the root process.
     *                  May have length zero on non-root tasks.
     * @param[in] root The root task that gathers the data.
     */
    template<typename T>
    static int gatherv (T* in, int sendlen, T* out, int* recvlen, int* displ, int root)
    {
        for (int i=*displ; i<sendlen; i++)
            out[i] = in[i];
        return 0;
    }

    /** @brief Scatter array from a root to all other task.
     *
     * The root process sends the elements with index from k*len to
     * (k+1)*len-1 in its array to task k, which stores it at index 0
     * to len-1.
     *
     * @param[in] send The array to scatter. Might have length zero on non-root
     *                  tasks.
     * @param[out] recv The buffer to store the received data in. Upon completion of the
     *                 method each task will have same data stored there as the one in
     *                 send buffer of the root task before.
     * @param[in] len The number of elements in the recv buffer.
     * @param[in] root The root task that gathers the data.
     */
    template<typename T>
    static int scatter (T* send, T* recv, int len, int root) // note out must have same size as in
    {
        for (int i=0; i<len; i++)
            recv[i] = send[i];
        return 0;
    }

    /** @brief Scatter arrays of variable length from a root to all other tasks.
     *
     * The root process sends the elements with index from
     * send+displ[k] to send+displ[k]-1 in * its array to task k,
     * which stores it at index 0 to recvlen-1.
     *
     * @param[in] send The array to scatter. May have length zero on non-root
     *                  tasks.
     * @param[in] sendlen An array with size equal to the number of processes containing the number
     *                    of elements to scatter to process i at position i, i.e. the number that
     *                    is passed as recvlen argument to this function in process i.
     * @param[in] displ An array with size equal to the number of processes. Data scattered to
     *                  process i will be read starting at send+displ[i] on root the process.
     * @param[out] recv The buffer to store the received data in. Upon completion of the
     *                  method each task will have the same data stored there as the one in
     *                  send buffer of the root task before.
     * @param[in] recvlen The number of elements in the recv buffer.
     * @param[in] root The root task that gathers the data.
     */
    template<typename T>
    static int scatterv (T* send, int* sendlen, int* displ, T* recv, int recvlen, int root)
    {
        for (int i=*displ; i<*sendlen; i++)
            recv[i] = send[i];
        return 0;
    }

    /**
     * @brief Gathers data from all tasks and distribute it to all.
     *
     * The block of data sent from the  jth  process  is  received  by  every
     *  process and placed in the jth block of the buffer recvbuf.
     *
     * @param[in] sbuf The buffer with the data to send. Has to be the same for
     *                 each task.
     * @param[in] count The number of elements to send by any process.
     * @param[out] rbuf The receive buffer for the data. Has to be of size
     *  notasks*count, with notasks being the number of tasks in the communicator.
     */
    template<typename T>
    static int allgather(T* sbuf, int count, T* rbuf)
    {
        for(T* end=sbuf+count; sbuf < end; ++sbuf, ++rbuf)
            *rbuf=*sbuf;
        return 0;
    }

    /**
     * @brief Gathers data of variable length from all tasks and distribute it to all.
     *
     * The block of data sent from the jth process is received by every
     *  process and placed in the jth block of the buffer out.
     *
     * @param[in] in The send buffer with the data to send.
     * @param[in] sendlen The number of elements to send on each task.
     * @param[out] out The buffer to store the received data in.
     * @param[in] recvlen An array with size equal to the number of processes containing the number
     *                    of elements to recieve from process i at position i, i.e. the number that
     *                    is passed as sendlen argument to this function in process i.
     * @param[in] displ An array with size equal to the number of processes. Data recieved from
     *                  process i will be written starting at out+displ[i].
     */
    template<typename T>
    static int allgatherv (T* in, int sendlen, T* out, int* recvlen, int* displ)
    {
        for (int i=*displ; i<sendlen; i++)
            out[i] = in[i];
        return 0;
    }

    /**
     * @brief Compute something over all processes
     * for each component of an array and return the result
     * in every process.
     *
     * The template parameter BinaryFunction is the type of
     * the binary function to use for the computation
     *
     * @param inout The array to compute on.
     * @param len The number of components in the array
     */
    template<typename BinaryFunction, typename Type>
    static int allreduce(Type* inout, int len)
    {
        return 0;
    }

    /**
     * @brief Compute something over all processes
     * for each component of an array and return the result
     * in every process.
     *
     * The template parameter BinaryFunction is the type of
     * the binary function to use for the computation
     *
     * @param in The array to compute on.
     * @param out The array to store the results in.
     * @param len The number of components in the array
     */
    template<typename BinaryFunction, typename Type>
    static void allreduce(Type* in, Type* out, int len)
    {
        std::copy(in, in+len, out);
        return;
    }
};

#ifdef GISMO_WITH_MPI

/**
  @brief A parallel communicator class based on MPI
 
  @ingroup Mpi
 */
class GISMO_EXPORT gsMpiComm
{
    friend class gsMpi;

public:
    
    gsMpiComm() : rank_(-1), size_(0) { }

    gsMpiComm(const MPI_Comm & _comm)
    : m_comm(_comm)
    {
        if(_comm != MPI_COMM_NULL) 
        {
#           ifndef NDEBUG
            int initialized = 0;
            MPI_Initialized(&initialized);
            GISMO_ENSURE(1==initialized, 
                         "You must call gsMpi::init(..) in your main() function"
                         " before using gsMpiComm");
            MPI_Comm_set_errhandler(m_comm, ErrHandler);
#           endif
            MPI_Comm_rank(m_comm, &rank_);
            MPI_Comm_size(m_comm, &size_);
        }
        else
        {
            size_ = 0;
            rank_ =-1;
        }
    }
   
    gsMpiComm(const gsSerialComm &) : m_comm(MPI_COMM_SELF) { }
    
    /**
     * @brief The type of the mpi communicator.
     */
    typedef MPI_Comm Communicator;

    /**
     * @brief Returns the rank of process
     */
    int rank () const { return rank_; }

    /**
     * @brief Returns the number of processes
     */
    int size () const { return size_; }

    /**
     * @brief Returns the name of the communicator
     */
    std::string name() const 
    { 
        char str[MPI_MAX_OBJECT_NAME];
        int len;
        MPI_Comm_get_name(m_comm, str, &len);
        return std::string(str, len);
    }

    operator MPI_Comm () const { return m_comm; }

private:
    int rank_;
    int size_;

    MPI_Comm m_comm;

#   ifndef NDEBUG
protected: 

    // Mpi error handling
    static void ErrCallBack(MPI_Comm *comm, int *err_code, ...)
    {
        char err_string[MPI_MAX_ERROR_STRING];
        int err_length, err_class;

        int rank;
        MPI_Comm_get_name(*comm, err_string, &err_length);
        MPI_Comm_rank(*comm, &rank);
        gsWarn << "MPI error ("<<*err_code<<") at process "<< rank 
               <<" of "<< err_string <<"\n";
        MPI_Error_class(*err_code, &err_class);
        MPI_Error_string(err_class, err_string, &err_length);
        gsWarn <<"gsMpi error class: "<<err_class <<" ("<< err_string <<")\n";
        MPI_Error_string(*err_code, err_string, &err_length);
        gsWarn <<"gsMpi error      : "<<*err_code <<" ("<< err_string <<")\n";
        throw std::runtime_error("GISMO_ERROR: " + std::string(err_string, err_length));
        //MPI_Abort(*comm, *err_code);
    }

    static MPI_Errhandler ErrHandler;
#   endif

public:

    /// @copydoc gsSerialComm::sum
    template<typename T>
    T sum (T& in) const
    {
        T out;
        allreduce<std::plus<T> >(&in,&out,1);
        return out;
    }

    /// @copydoc gsSerialComm::sum
    template<typename T>
    int sum (T* inout, int len) const
    {
        return allreduce<std::plus<T> >(inout,len);
    }

    template<typename T>
    int sum (T* inout, int len, int root) const
    {
        return reduce<std::plus<T> >(inout,len,root);
    }

    template<typename T>
    int sum (T* in, T* out, int len, int root) const
    {
        return reduce<std::plus<T> >(in,out,len,root);
    }

    template<typename T>
    int isum (T* in,T* out, int len, int root, MPI_Request* req) const
    {
        return iallreduce<std::plus<T> >(in, out,len,req);
    }

    template<typename T>
    int isum (T* inout, int len, int root, MPI_Request* req) const
    {
        return ireduce<std::plus<T> >(inout,len,root,req);
    }

    template<typename T>
    int isum (T* inout, int len, MPI_Request* req) const
    {
        return iallreduce<std::plus<T> >(inout,len,req);
    }


    /// @copydoc gsSerialComm::prod
    template<typename T>
    T prod (T& in) const
    {
        T out;
        allreduce<std::multiplies<T> >(&in,&out,1);
        return out;
    }

    /// @copydoc gsSerialComm::prod
    template<typename T>
    int prod (T* inout, int len) const
    {
        return allreduce<std::multiplies<T> >(inout,len);
    }

    /// @copydoc gsSerialComm::min
    template<typename T>
    T min (T& in) const
    {
        T out;
        allreduce<Min<T> >(&in,&out,1);
        return out;
    }

    /// @copydoc gsSerialComm::min
    template<typename T>
    int min (T* inout, int len) const
    {
        return allreduce<Min<T> >(inout,len);
    }


    /// @copydoc gsSerialComm::max
    template<typename T>
    T max (T& in) const
    {
        T out;
        allreduce<Max<T> >(&in,&out,1);
        return out;
    }

    /// @copydoc gsSerialComm::max
    template<typename T>
    int max (T* inout, int len) const
    {
        return allreduce<Max<T> >(inout,len);
    }

    /// @copydoc gsSerialComm::barrier
    int barrier () const
    {
        return MPI_Barrier(m_comm);
    }

    /// @copydoc gsSerialComm::broadcast
    template<typename T>
    int broadcast (T* inout, int len, int root) const
    {
        return MPI_Bcast(inout,len,MPITraits<T>::getType(),root,m_comm);
    }

    /// @copydoc gsSerialComm::gather()
    /// @note out must have space for P*len elements
    template<typename T>
    int gather (T* in, T* out, int len, int root) const
    {
        return MPI_Gather(in,len,MPITraits<T>::getType(),
                          out,len,MPITraits<T>::getType(),
                          root,m_comm);
    }

    /// @copydoc gsSerialComm::gatherv()
    template<typename T>
    int gatherv (T* in, int sendlen, T* out, int* recvlen, int* displ, int root) const
    {
        return MPI_Gatherv(in,sendlen,MPITraits<T>::getType(),
                           out,recvlen,displ,MPITraits<T>::getType(),
                           root,m_comm);
    }

    /// @copydoc gsSerialComm::scatter()
    /// @note out must have space for P*len elements
    template<typename T>
    int scatter (T* send, T* recv, int len, int root) const
    {
        return MPI_Scatter(send,len,MPITraits<T>::getType(),
                           recv,len,MPITraits<T>::getType(),
                           root,m_comm);
    }

    /// @copydoc gsSerialComm::scatterv()
    template<typename T>
    int scatterv (T* send, int* sendlen, int* displ, T* recv, int recvlen, int root) const
    {
        return MPI_Scatterv(send,sendlen,displ,MPITraits<T>::getType(),
                            recv,recvlen,MPITraits<T>::getType(),
                            root,m_comm);
    }

    /// @copydoc gsSerialComm::allgather()
    template<typename T, typename T1>
    int allgather(T* sbuf, int count, T1* rbuf) const
    {
        return MPI_Allgather(sbuf, count, MPITraits<T>::getType(),
                             rbuf, count, MPITraits<T1>::getType(),
                             m_comm);
    }

    /// @copydoc gsSerialComm::allgatherv()
    template<typename T>
    int allgatherv (T* in, int sendlen, T* out, int* recvlen, int* displ) const
    {
        return MPI_Allgatherv(in,sendlen,MPITraits<T>::getType(),
                              out,recvlen,displ,MPITraits<T>::getType(),
                              m_comm);
    }

#ifndef MPI_IN_PLACE
 #define MPI_IN_PLACE inout
 #define MASK_MPI_IN_PLACE
/*
#  ifdef _MSC_VER
#    pragma message ("Masking MPI_IN_PLACE (not found in MPI version used).")
#  else
#    warning Masking MPI_IN_PLACE (not found in MPI version used).
#  endif
*/
#endif
    
    /// @copydoc gsSerialComm::allreduce(Type* inout,int len) const
    template<typename BinaryFunction, typename Type>
    int allreduce(Type* inout, int len) const
    {
        // Type* out = new Type[len];
        // int ret = allreduce<BinaryFunction>(inout,out,len);
        // std::copy(out, out+len, inout);
        // delete[] out;
        // return ret;
        return MPI_Allreduce(MPI_IN_PLACE, inout, len, MPITraits<Type>::getType(),
                             (Generic_MPI_Op<Type, BinaryFunction>::get()),m_comm);
    }


    /// @copydoc gsSerialComm::allreduce(Type* in,Type* out,int len) const
    template<typename BinaryFunction, typename Type>
    int allreduce(Type* in, Type* out, int len) const
    {
        return MPI_Allreduce(in, out, len, MPITraits<Type>::getType(),
                             (Generic_MPI_Op<Type, BinaryFunction>::get()),m_comm);
    }



    /// @copydoc gsSerialComm::allreduce(Type* in,Type* out,int len) const
    template<typename BinaryFunction, typename Type>
    int iallreduce(Type* in, Type* out, int len,MPI_Request* req) const
    {
        return MPI_Iallreduce(in, out, len, MPITraits<Type>::getType(),
                              (Generic_MPI_Op<Type, BinaryFunction>::get()),m_comm,req);
    }

    /// @copydoc gsSerialComm::allreduce(Type* inout,int len) const
    template<typename BinaryFunction, typename Type>
    int iallreduce(Type* inout, int len,MPI_Request* req) const
    {
        return MPI_Iallreduce(MPI_IN_PLACE, inout, len, MPITraits<Type>::getType(),
                              (Generic_MPI_Op<Type, BinaryFunction>::get()),m_comm,req);
    }

    template<typename BinaryFunction, typename Type>
    int reduce(Type* inout, int len,int root) const
    {
        int ret;
        if(root == rank())
            ret = MPI_Reduce(MPI_IN_PLACE, inout, len, MPITraits<Type>::getType(),
                             (Generic_MPI_Op<Type, BinaryFunction>::get()),root,m_comm);
        else
            ret = MPI_Reduce(inout, NULL, len, MPITraits<Type>::getType(),
                             (Generic_MPI_Op<Type, BinaryFunction>::get()),root,m_comm);
        return ret;
    }

    template<typename BinaryFunction, typename Type>
    int reduce(Type* in,Type* out, int len,int root) const
    {
        return MPI_Reduce(in, out, len, MPITraits<Type>::getType(),
                          (Generic_MPI_Op<Type, BinaryFunction>::get()),root,m_comm);
    }

    template<typename BinaryFunction, typename Type>
    int ireduce(Type* inout, int len,int root, MPI_Request* req) const
    {
        int ret;
        if(root == rank())
            ret = MPI_Ireduce(MPI_IN_PLACE, inout, len, MPITraits<Type>::getType(),
                              (Generic_MPI_Op<Type, BinaryFunction>::get()),root,m_comm,req);
        else
            ret = MPI_Ireduce(inout, inout, len, MPITraits<Type>::getType(),
                              (Generic_MPI_Op<Type, BinaryFunction>::get()),root,m_comm,req);
        return ret;
    }

#ifdef MASK_MPI_IN_PLACE
#undef MPI_IN_PLACE
#undef MASK_MPI_IN_PLACE
#endif
    
    template<typename BinaryFunction, typename Type>
    int ireduce(Type* in, Type* out, int len, int root, MPI_Request* req) const
    {
        return MPI_Ireduce(in, out, len, MPITraits<Type>::getType(),
                           (Generic_MPI_Op<Type, BinaryFunction>::get()),root,m_comm,req);
    }

};

#else
// If we compile without MPI, then all we have is the gsSerialComm
typedef gsSerialComm gsMpiComm;
#endif

}

