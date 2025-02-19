//------------------------------------------------------------------------------
// GB_assign_scalar:    C<M>(Rows,Cols) = accum (C(Rows,Cols),x)
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017-2022, All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

//------------------------------------------------------------------------------

// Assigns a single scalar to a submatrix:

// C<M>(Rows,Cols) = accum (C(Rows,Cols),x)

// This function does the work for GrB_Matrix_assign_TYPE and
// GrB_Vector_assign_[type], where [type] is one of the 11 types, or the
// type-generic macro suffix, "_UDT".

// Compare with GB_subassign_scalar, which uses M and C_replace differently

#include "GB_assign.h"
#include "GB_bitmap_assign.h"
#include "GB_get_mask.h"

GrB_Info GB_assign_scalar           // C<M>(Rows,Cols) += x
(
    GrB_Matrix C,                   // input/output matrix for results
    const GrB_Matrix M_in,          // mask for C(Rows,Cols), unused if NULL
    const GrB_BinaryOp accum,       // accum for Z=accum(C(Rows,Cols),T)
    const void *scalar,             // scalar to assign to C(Rows,Cols)
    const GB_Type_code scalar_code, // type code of scalar to assign
    const GrB_Index *Rows,          // row indices
    const GrB_Index nRows,          // number of row indices
    const GrB_Index *Cols,          // column indices
    const GrB_Index nCols,          // number of column indices
    const GrB_Descriptor desc,      // descriptor for C and M
    GB_Context Context
)
{ 

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    GB_RETURN_IF_NULL (scalar) ;
    ASSERT (scalar_code <= GB_UDT_code) ;

    // get the descriptor
    GB_GET_DESCRIPTOR (info, desc, C_replace, Mask_comp, Mask_struct,
        xx1, xx2, xx3, xx7) ;

    // get the mask
    GrB_Matrix M = GB_get_mask (M_in, &Mask_comp, &Mask_struct) ;

    //--------------------------------------------------------------------------
    // C<M>(Rows,Cols) = accum (C(Rows,Cols), scalar)
    //--------------------------------------------------------------------------

    return (GB_assign (
        C, C_replace,               // C matrix and its descriptor
        M, Mask_comp, Mask_struct,  // mask matrix and its descriptor
        false,                      // do not transpose the mask
        accum,                      // for accum (C(Rows,Cols),scalar)
        NULL, false,                // no explicit matrix A
        Rows, nRows,                // row indices
        Cols, nCols,                // column indices
        true,                       // do scalar expansion
        scalar,                     // scalar to assign, expands to become A
        scalar_code,                // type code of scalar to expand
        GB_ASSIGN,
        Context)) ;
}

