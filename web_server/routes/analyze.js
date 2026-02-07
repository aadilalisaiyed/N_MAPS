import express from "express";
import axios from "axios";

const router = express.Router();

/**
 * POST /api/analyze
 * Body: { "message": "text here" }
 */
router.post("/analyze", async (req, res) => {
  console.log(" Request reached Node.js");
  console.log(" Body:", req.body);

  const { message } = req.body;

  // basic validation
  if (!message) {
    return res.status(400).json({
      error: "message field is required"
    });
  }

  try {
    // CALL ML SERVER
    const mlResponse = await axios.post(
      "http://127.0.0.1:8000/predict",
      { message },
      { timeout: 5000 }
    );

    console.log(" ML response:", mlResponse.data);

    // SEND BACK TO CLIENT
    return res.json({
      success: true,
      prediction: mlResponse.data.prediction,
      confidence: mlResponse.data.confidence
    });

  } catch (error) {
    console.error(" Error talking to ML server");

    if (error.response) {
      console.error("ML error response:", error.response.data);
    } else if (error.request) {
      console.error("No response from ML server");
    } else {
      console.error("Axios error:", error.message);
    }

    return res.status(500).json({
      success: false,
      error: "ML server not reachable"
    });
  }
});

export default router;
