#ifndef JOB_STATUS_H
#define JOB_STATUS_H

#include <string>

enum class JobStatus {
    PENDING,
    PROCESSING,
    PAUSED,
    COMPLETED,
    CANCELED,
    JOB_ERROR,
    WAITING_FOR_DEVICE,
    UNKNOWN
};

inline std::string to_string(JobStatus status) {
    switch (status) {
        case JobStatus::PENDING: return "pending";
        case JobStatus::PROCESSING: return "processing";
        case JobStatus::PAUSED: return "paused";
        case JobStatus::COMPLETED: return "completed";
        case JobStatus::CANCELED: return "canceled";
        case JobStatus::JOB_ERROR: return "error";
        case JobStatus::WAITING_FOR_DEVICE: return "waiting for device";
        default: return "unknown";
    }
}

#endif
